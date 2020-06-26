#pragma once

#include <functional>
#include "../Publisher/Instance.hpp"
#include "../Reducer/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Store {

    template <class State, class Params, size_t size>
    class Instance : public Interface<State, Params> {

      public:

        using Subscriber = Subscriber::Interface<State>;
        using Subscribers = std::array<Subscriber *, size>;
        using Reducer = Reducer::Interface<State, Params>;

        Instance(Reducer & reducer, Subscribers subscribers) :
          publisher(subscribers),
          reducer(reducer),
          reducing(false),
          notifying(false),
          settingUp(false),
          deserializing(false),
          initializing(false),
          nextState(nullptr)
        {}

        void deserialize(const JsonObject & serialized, Params & params) override {
          deserializing = true;
          reducer.deserialize(serialized, params);
          deserializing = false;
          init(params);
        }

        void init(const Params & params) override {
          initializing = true;
          const State * initial = reducer.init(params);
          initializing = false;
          setup(initial);
        }

        void setup(const State * initial) override {
          settingUp = true;
          publisher.setup(initial);
          settingUp = false;
        }

        void loop() override {
          // Notify asynchronously so that 
          // actions can be batched synchronously.
          // State reduction is always synchronous
          if (nextState) {
            publish(nextState);
          }
        }

        Error dispatch(const Action::Interface & action) override {
          if (deserializing) {
            // prevent dispatch during deserialization and report error
            return Error::dispatchDuringDeserializeError;
          }
          if (initializing) {
            // prevent dispatch during initialization and report error
            return Error::dispatchDuringInitError;
          }
          if (reducing) {
            // prevent dispatch during reduce and report error
            return Error::dispatchDuringReduceError;
          }
          Error error = Error::noError;
          if (settingUp) {
            // don't prevent dispatch during setup but
            // report warning so that users can detect when it happens
            error = Error::dispatchDuringSetupWarning;
          }
          if (notifying) {
            // don't prevent dispatch during notification but
            // report warning so that users can detect when it happens
            error = Error::dispatchDuringNotificationWarning;
          }
          reducing = true;
          nextState = reducer.reduce(publisher.getState(), action);
          reducing = false;
          return error;
        }

        const State * getState() const override {
          return publisher.getState();
        }

      private:

        Publisher::Instance<State, size> publisher;
        Reducer & reducer;
        bool reducing;
        bool notifying;
        bool settingUp;
        bool deserializing;
        bool initializing;
        const State * nextState;

        void publish(const State * state) override {
          nextState = nullptr;
          notifying = true;
          publisher.publish(state);
          notifying = false;
        }

    };

  }
}
