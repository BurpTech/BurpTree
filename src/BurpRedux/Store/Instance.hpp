#pragma once

#include "../Publisher/Instance.hpp"
#include "Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {
  namespace Store {

    template <class State, size_t subscriberCount>
    class Instance : public Interface {

      public:

        using StateList = Interface::StateList;
        using Id = Interface::Id;
        using StateInterface = Interface::StateInterface;
        using Reducer = Reducer::Interface;
        using Publisher = Publisher::Instance<State, subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;

        Instance(Reducer & reducer, Subscribers subscribers) :
          publisher(subscribers),
          reducer(reducer),
          reducing(false),
          notifying(false),
          initializing(false),
          nextState(nullptr)
        {}

        void init(const StateList & list) override {
          initializing = true;
          StateInterface * initial = reducer.init(list);
          publisher.setup(initial->template as<State>());
          initializing = false;
        }

        void loop() override {
          // Notify asynchronously so that 
          // actions can be batched synchronously.
          // State reduction is always synchronous
          if (nextState) {
            StateInterface * state = nextState;
            nextState = nullptr;
            notifying = true;
            publisher.publish(state->template as<State>());
            notifying = false;
          }
        }

        const Status & dispatch(const Id id, StateInterface * next) override {
          if (initializing) {
            // prevent dispatch during initialization and report error
            status.set(Status::Level::ERROR, Status::dispatchDuringInit);
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

        State * getState() const {
          return publisher.getState();
        }

      private:

        Publisher publisher;
        Reducer & reducer;
        bool reducing;
        bool notifying;
        bool initializing;
        StateInterface * nextState;
        Status status;

    };

  }
}
