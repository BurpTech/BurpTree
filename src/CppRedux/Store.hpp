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

      Store(const Reducer<State, Action> & reducer) :
        _state(nullptr),
        _lastState(nullptr),
        _reducer(reducer),
        _subscriber(nullptr),
        _reducing(false),
        _notifying(false)
      {}

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

      void init(const State & initialState) {
        // Passing the state to _reducer->init 
        // allows us to reinitialize the reducer
        // without leaking memory as the state
        // will not be nullptr if init is called
        // twice
        _reducing = true;
        _state = _reducer.init(_state, initialState);
        // prevent notification from init as subscribers
        // should be added afterwards anyway and they should
        // also be init'd if required to read the state
        _lastState = _state;
        _reducing = false;
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
        _reducing = true;
        _state = _reducer.reduce(_state, action);
        _reducing = false;
        return ret;
      }

      const State * getState() {
        return _state;
      }

    private:

      const State * _state;
      const State * _lastState;
      const Reducer<State, Action> & _reducer;
      Subscriber * _subscriber;
      bool _reducing;
      bool _notifying;

  };
}
