#pragma once

#include <ArduinoJson.h>
#include "../Action/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class State>
    class Interface {

      public:

        virtual const State * deserialize(const JsonObject & serialized) = 0;
        virtual const State * reduce(const State * previous, const Action::Interface & action) = 0;

    };

  }
}
