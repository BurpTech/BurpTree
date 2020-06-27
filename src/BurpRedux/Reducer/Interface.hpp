#pragma once

#include "../StateList/Interface.hpp"
#include "../State/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Interface {

      public:

        using Id = unsigned int;
        using StateList = StateList::Interface;
        using State = State::Interface;

        virtual const State * init(const StateList & list) const = 0;
        virtual void deserialize(State * current, const JsonObject & serialized) const = 0;
        virtual const State * reduce(const Id id, const State * previous, const State * next) const = 0;

    };

  }
}
