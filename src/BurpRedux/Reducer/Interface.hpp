#pragma once

#include "../State/Interface.hpp"
#include "../State/List/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Interface {

      public:

        using Id = unsigned int;
        using StateList = State::List::Interface;
        using State = State::Interface;

        virtual State * init(const StateList & list) = 0;
        virtual State * reduce(const Id id, State * previous, State * next) = 0;

    };

  }
}
