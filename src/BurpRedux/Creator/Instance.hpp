#pragma once

#include <memory>
#include "Interface2.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State, class Params>
    class Instance : public Interface2<State, Params> {

      public:

        State * init(const Params & params) override {
          _uid++;
          State * next = new(&(_pair[_current])) State(params, _uid);
          _increment();
          return next;
        }

        State * begin(const State * previous) override {
          _uid++;
          return new(&(_pair[_current])) State(previous, _uid);
        }

        void commit(const State * previous) override {
          _increment();
          previous->~State();
        }

      private:

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
