#pragma once

#include <functional>
#include "./Action.hpp"

namespace CppRedux {

  template <class State, class Action>
  class Reducer {

    public:

      virtual const State * init(const State * previous, const State & initialState) const = 0;
      virtual const State * reduce(const State * previous, const Action & action) const = 0;

      using f_alloc = std::function<const State * (void * address)>;

      const State * update(const State * previous, const State & next) const {
        auto address = previous == state1 ? state2 : state1;
        if (previous) {
          State * ret = new(address) State(previous, next);
          previous->~State();
          return ret;
        }
        return new(address) State(next);
      }

    private:

      void * state1 = ::operator new(sizeof(State));
      void * state2 = ::operator new(sizeof(State));

  };
}
