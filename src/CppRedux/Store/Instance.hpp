#pragma once

#include <functional>
#include "../Publisher/Instance.hpp"
#include "Interface.hpp"

namespace CppRedux {
  namespace Store {

    template <class State, class Action, size_t size>
    class Instance : public Interface<State, Action> {

      public:

        using f_reducer = std::function<const State * (const State * state, const Action & action)>;

        Instance(f_reducer reducer, const State * state) :
          reducer(reducer),
          publisher(state),
          reducing(false),
          notifying(false),
          nextState(nullptr)
        {}

        void loop() override {
          // Notify asynchronously so that 
          // actions can be batched synchronously.
          // State reduction is always synchronous
          if (nextState) {
            publish(nextState);
          }
        }

        Error dispatch(const Action & action) override {
          if (reducing) {
            // prevent dispatch during reduce and report error
            return Error::dispatchDuringReduceError;
          }
          Error error = Error::noError;
          if (notifying) {
            // don't prevent dispatch during notification but
            // report warning so that users can detect when it happens
            error = Error::dispatchDuringNotificationWarning;
          }
          if (reducer) {
            reducing = true;
            nextState = reducer(publisher.getState(), action);
            reducing = false;
          }
          return error;
        }

        bool subscribe(Subscriber<State> * subscriber) override {
          return publisher.subscribe(subscriber);
        }

        const State * getState() const override {
          return publisher.getState();
        }

        size_t getSubscriberCount() const override {
          return publisher.getSubscriberCount();
        }
        
      private:

        f_reducer reducer;
        Publisher::Instance<State, size> publisher;
        bool reducing;
        bool notifying;
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
