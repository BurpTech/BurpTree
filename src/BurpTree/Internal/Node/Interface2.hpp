#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"
#include "Id.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      class Interface2 {

        public:

          virtual const State::Interface * setup(const JsonVariant & object) = 0;
          virtual const State::Interface * update(const Id changed) = 0;
          virtual void notify() = 0;

      };

    }
  }
}
