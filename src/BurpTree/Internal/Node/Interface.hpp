#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"
#include "Id.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      class Interface {

        public:

          virtual const State::Interface * setup(const JsonObject & object) = 0;
          virtual const State::Interface * update(const Id changed) = 0;
          virtual void notify() = 0;

      };

    }
  }
}
