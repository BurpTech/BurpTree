#pragma once

#include <functional>

namespace CppRedux {

  template <class Store, class InputState, class ReturnState>
  class Selector {
    
    public:

      using f_select = std::function<const ReturnState * (const InputState * inputState)>;
      using f_onChange = std::function<void(const ReturnState * returnState)>;

      Selector(const Store & store, f_select select) :
        _store(store),
        _select(select),
        _state(nullptr)
      {}

      void check(f_onChange onChange) {
        const ReturnState * state = _select(_store.getState());
        if (state != _state) {
          _state = state;
          onChange(_state);
        }
      }

      const ReturnState * get() const {
        return _state;
      }

      void reset() {
        _state = nullptr;
      }

    private:

      const Store & _store;
      f_select _select;
      const ReturnState * _state;

  };

}
