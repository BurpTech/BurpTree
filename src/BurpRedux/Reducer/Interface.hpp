#pragma once

namespace BurpRedux {
  namespace Reducer {

    template <class State, class Action>
    class Interface {

      public:

        virtual const State * reduce(const State * state, const Action & action) = 0;

    };

  }
}
