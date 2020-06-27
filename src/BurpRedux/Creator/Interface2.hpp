#pragma once

#include "Interface1.hpp"

namespace BurpRedux {
  namespace Creator {

    template <class State, class Params>
    class Interface2 : public Interface1<State> {

      public:

        virtual State * init(const Params & params) = 0;

    };

  }
}
