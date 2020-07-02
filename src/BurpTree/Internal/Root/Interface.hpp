#pragma once

#include <ArduinoJson.h>
#include "../Node/Id.hpp"
#include "Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      class Interface {

        public:

          using Id = Node::Id;

          virtual void deserialize(const JsonObject & object) = 0;
          virtual void loop() = 0;
          virtual const Status & dispatch(const Id id) = 0;

      };

    }
  }
}
