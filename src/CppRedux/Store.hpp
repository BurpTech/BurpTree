#pragma once

#include "Reducer.hpp"
#include "Subscriber.hpp"
#include "Action.hpp"

namespace CppRedux {

  enum class Error {
    noError,
    dispatchDuringNotificationWarning,
    dispatchDuringReduceError
  };

  template <class State, class Action>
  class Store {

    public:

      Store() :
        _state(nullptr),
        _lastState(nullptr),
        _reducer(nullptr),
        _subscriber(nullptr),
        _reducing(false),
        _notifying(false)
      {}

      void setReducer(const Reducer<State, Action> * reducer, const State * initialState) {
        _reducer = reducer;
        _state = initialState;
        // prevent notification from init as subscribers
        // should be added afterwards anyway and they should
        // also be init'd if required to read the state
        _lastState = _state;
      }

      void setSubscriber(Subscriber * subscriber) {
        _subscriber = subscriber;
      }

      void loop() {
        // Notify asynchronously so that 
        // actions can be batched synchronously.
        // State reduction is always synchronous
        if (_state != _lastState) {
          _notifying = true;
          if (_subscriber) {
            _subscriber->notify();
          }
          _lastState = _state;
          _notifying = false;
        }
      }

      Error dispatch(const Action & action) {
        if (_reducing) {
          // prevent dispatch during reduce and report error
          return Error::dispatchDuringReduceError;
        }
        Error ret = Error::noError;
        if (_notifying) {
          // don't prevent dispatch during notification but
          // report warning so that users can detect when it happens
          ret = Error::dispatchDuringNotificationWarning;
        }
        if (_reducer) {
          _reducing = true;
          _state = _reducer->reduce(_state, action);
          _reducing = false;
        }
        return ret;
      }

      const State * getState() const {
        return _state;
      }

    private:

      const State * _state;
      const State * _lastState;
      const Reducer<State, Action> * _reducer;
      Subscriber * _subscriber;
      bool _reducing;
      bool _notifying;

  };
}
