#pragma once

#include <ArduinoJson.h>
#include "../Action/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class State, class Params>
    class Interface {

      public:

        virtual void deserialize(const JsonObject & serialized, Params & params) = 0;
        virtual const State * init(const Params & params) = 0;
        virtual const State * reduce(const State * previous, const Action::Interface & action) = 0;

    };

  }
}
