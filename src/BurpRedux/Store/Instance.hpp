#pragma once

#include "../Publisher/Instance.hpp"
#include "Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {
  namespace Store {

    template <class State, size_t subscriberCount>
    class Instance : public Interface<State> {

      public:

        using Id = typename Interface<State>::Id;
        using StateInterface = typename Interface<State>::StateInterface;
        using Reducer = Reducer::Interface;
        using Publisher = Publisher::Instance<State, subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;

        Instance(Reducer & reducer, const Subscribers subscribers) :
          publisher(subscribers),
          reducer(reducer),
          reducing(false),
          notifying(false),
          deserializing(false),
          nextState(nullptr)
        {}

        void deserialize(const JsonObject & object) override {
          deserializing = true;
          const StateInterface * initial = reducer.deserialize(object);
          publisher.setup(initial->template as<State>());
          deserializing = false;
        }

        void loop() override {
          // Notify asynchronously so that 
          // actions can be batched synchronously.
          // State reduction is always synchronous
          if (nextState) {
            const StateInterface * state = nextState;
            nextState = nullptr;
            notifying = true;
            publisher.publish(state->template as<State>());
            notifying = false;
          }
        }

        const Status & dispatch(const Id id, const StateInterface * next) override {
          if (deserializing) {
            // prevent dispatch during deserialization and report error
            status.set(Status::Level::ERROR, Status::dispatchDuringDeserialize);
            return status;
          }
          if (reducing) {
            // prevent dispatch during reduce and report error
            status.set(Status::Level::ERROR, Status::dispatchDuringReduce);
            return status;
          }
          status.set(Status::Level::INFO, Status::noError);
          if (notifying) {
            // don't prevent dispatch during notification but
            // report warning so that users can detect when it happens
            status.set(Status::Level::WARNING, Status::dispatchDuringNotification);
          }
          reducing = true;
          nextState = reducer.reduce(id, publisher.getState(), next);
          reducing = false;
          return status;
        }

        const State * getState() const override {
          return publisher.getState();
        }

      private:

        Publisher publisher;
        Reducer & reducer;
        bool reducing;
        bool notifying;
        bool deserializing;
        const StateInterface * nextState;
        Status status;

    };

  }
}
