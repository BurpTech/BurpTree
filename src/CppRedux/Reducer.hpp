#pragma once

#include <functional>
#include "./Action.hpp"

namespace CppRedux {

  template <class State, class Action>
  class Reducer {

    public:

      virtual const State * reduce(const State * previous, const Action & action) const = 0;

  };
}
