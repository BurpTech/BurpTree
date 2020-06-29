#pragma once

#include "Base.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State, class InitParams>
    class WithInitParams : public Base<State> {

      public:

        State * init(const InitParams params) {
          Base<State>::_uid++;
          State * next = new(&(Base<State>::_pair[Base<State>::_current])) State(params, Base<State>::_uid);
          Base<State>::_increment();
          return next;
        }

    };

  }
}
