#pragma once

#include <functional>

namespace CppRedux {

  template <class InputState, class ReturnState>
  class Selector {
    
    public:

      using f_select = std::function<const ReturnState * (const InputState * inputState)>;
      using f_onChange = std::function<void(const ReturnState * returnState)>;

      Selector(f_select select) :
        _select(select),
        _state(nullptr)
      {}

      void check(const InputState * inputState, f_onChange onChange) {
        const ReturnState * state = _select(inputState);
        if (state != _state) {
          _state = state;
          onChange(state);
        }
      }

      void reset() {
        _state = nullptr;
      }

    private:

      f_select _select;
      const ReturnState * _state;

  };

}
