#pragma once

#include <functional>
#include "Subscriber.hpp"

namespace CppRedux {

  template <class Store, class InputState, class ReturnState>
  class Selector : public Subscriber {
    
    public:

      using f_select = std::function<const ReturnState * (const InputState * inputState)>;

      Selector(const Store & store, f_select select) :
        _store(store),
        _select(select),
        _state(nullptr)
      {}

      void notify() override {
        const ReturnState * state = _select(_store.getState());
        if (state != _state) {
          _state = state;
          if (_subscriber) {
            _subscriber->notify();
          }
        }
      }

      void setSubscriber(Subscriber * subscriber) {
        _subscriber = subscriber;
      }

      const ReturnState * getState() const {
        return _state;
      }

      void reset() {
        _state = nullptr;
      }

    private:

      const Store & _store;
      f_select _select;
      const ReturnState * _state;
      Subscriber * _subscriber;

  };

}
