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

          virtual bool setup(const JsonVariant & serialized) = 0;
          virtual void loop() = 0;
          virtual const Status & update(const Id changed) = 0;

      };

    }
  }
}
