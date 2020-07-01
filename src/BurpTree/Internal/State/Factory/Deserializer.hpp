#pragma once

#include <ArduinoJson.h>
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        class Deserializer {

          public:

            virtual const Base * deserialize(const JsonObject & serialized) = 0;

        };

      }
    }
  }
}
