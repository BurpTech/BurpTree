#pragma once

#include <ArduinoJson.h>

namespace BurpRedux {
  namespace State {

    class Interface {

      public:

        virtual const unsigned long getUid() const = 0;
        virtual void serialize(const JsonObject & serialized) const = 0;

    };

  }
}
