#pragma once

#include <functional>
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <class State>
        class Pool {

          protected:

            using Uid = Base::Uid;
            using f_construct = std::function<State * (const Uid uid, void * address)>;

            const State * _create(f_construct construct) {
              State * state = construct(_uid++, &(_memory[_current]));
              if (state) {
                _current++;
                _current %= _size;
                _setPrevious(state);
              }
              return state;
            };

            void _setPrevious(State * state) {
              if (_previous) {
                _previous->~State();
              }
              _previous = state;
            }

            static constexpr size_t _size = 2;
            unsigned char _current = 0;
            State * _previous = nullptr;
            Uid _uid;
            char _memory[_size][sizeof(State)];

        };

      }
    }
  }
}
