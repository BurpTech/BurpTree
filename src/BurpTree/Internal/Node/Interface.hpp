#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"
#include "Id.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      class Interface {

        public:

          virtual const State::Interface * deserialize(const JsonObject & object) = 0;
          virtual const State::Interface * dispatch(const Id id) = 0;
          virtual void notify() = 0;

      };

    }
  }
}
