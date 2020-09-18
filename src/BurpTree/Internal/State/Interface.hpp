#pragma once

#include <ArduinoJson.h>

namespace BurpTree {
  namespace Internal {
    namespace State {

      class Interface {

        public:

          virtual void serialize(const JsonVariant & serialized) const = 0;

          template <class Derived>
          const Derived * as() const {
            return static_cast<const Derived *>(this);
          }

      };

    }
  }
}
