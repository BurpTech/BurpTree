#pragma once

#include <ArduinoJson.h>
#include "../Action/Interface.hpp"

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class CombinedParams>
    class Interface {

      public:

        virtual void deserialize(const JsonObject & serialized, CombinedParams & params) = 0;
        virtual void init(CombinedState & state, const CombinedParams & params) = 0;
        virtual bool reduce(const CombinedState * previous, CombinedState & temp, const Action::Interface & action) = 0;

    };

  }
}
