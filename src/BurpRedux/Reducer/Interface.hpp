#pragma once

#include "../Action/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class State>
    class Interface {

      public:

        virtual const State * reduce(const State * previous, const Action::Interface & action) = 0;

    };

  }
}
