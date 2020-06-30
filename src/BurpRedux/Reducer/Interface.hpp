#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Interface {

      public:

        using Id = unsigned int;
        using State = State::Interface;

        virtual const State * deserialize(const JsonObject & object) = 0;
        virtual const State * reduce(const Id id, const State * previous, const State * next) = 0;

    };

  }
}
