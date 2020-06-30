#pragma once

#include <ArduinoJson.h>
#include "../Interface.hpp"

namespace BurpRedux {
  namespace State {
    namespace Factory {

      class Deserializer {

        public:

          virtual const Interface * deserialize(const JsonObject & serialized) = 0;

      };

    }
  }
}
