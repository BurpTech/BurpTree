#pragma once

#include <memory>
#include "Interface.hpp"

namespace BurpRedux {
  namespace Sequence {

    template <class State, class Params>
    class Instance : Interface<State> {

      public:

        Instance() :
          id(0),
          current(1),
          pair(allocator.allocate(2))
        {}

        void newState(const Params * params) {
          id++;
          // record the previous state so we can destroy
          // it later
          State * previous = &(pair[current]);
          current++;
          current %= 2;
          new(&(pair[current])) State(params);
          // we destroy the previous state later in case
          // it was referenced in the params
          previous->~State();
        }

        const State * getState() const override {
          return &(pair[current]);
        }

        unsigned long getId() const override {
          return id;
        }

      private:

        std::allocator<State> allocator;
        unsigned long id;
        unsigned char current;
        State * pair;

    };

  }
}
