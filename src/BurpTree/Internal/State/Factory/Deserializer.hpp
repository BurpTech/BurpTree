#pragma once

#include <ArduinoJson.h>
#include "../Base.hpp"
#include "../../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        class Deserializer {

          public:

            using State = Base;

            virtual const State * deserialize(const JsonObject & serialized) = 0;
            virtual const Status & getStatus() const = 0;

        };

      }
    }
  }
}
