#pragma once

#include <memory>
#include "Interface.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State, class Params>
    class Instance : public Interface<State, Params> {

      public:

        const State * init(const Params & params) override {
          return replace(nullptr, params);
        }

        const State * replace(const State * previous, const Params & params) override {
          _current++;
          _current %= 2;
          _uid++;
          const State * next = new(&(_pair[_current])) State(params, _uid);
          if (previous) {
            previous->~State();
          }
          return next;
        }

      private:

        std::allocator<State> allocator;
        State * _pair = allocator.allocate(2);
        size_t _current = 1;
        unsigned long _uid = 0;

    };

  }
}
