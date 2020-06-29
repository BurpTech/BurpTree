#pragma once

#include <memory>
#include "Interface.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State>
    class Base : public Interface<State> {

      public:

        State * begin(const State * previous) override {
          _uid++;
          return new(&(_pair[_current])) State(previous, _uid);
        }

        void commit(const State * previous) override {
          _increment();
          previous->~State();
        }

      protected:

        std::allocator<State> allocator;
        State * _pair = allocator.allocate(2);
        size_t _current = 0;
        unsigned long _uid = 0;

        void _increment() {
          _current++;
          _current %= 2;
        }

    };

  }
}
