#pragma once

#include <functional>

namespace BurpRedux {

  template <class State, class Action>
  class Reducer {

    public:

      virtual const State * reduce(const State * state, const Action & action) = 0;

  };

}
