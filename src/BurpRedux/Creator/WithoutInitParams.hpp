#pragma once

#include "Base.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State>
    class WithoutInitParams : public Base<State> {

      public:

        State * init() {
          Base<State>::_uid++;
          State * next = new(&(Base<State>::_pair[Base<State>::_current])) State(nullptr, Base<State>::_uid);
          Base<State>::_increment();
          return next;
        }

    };

  }
}
