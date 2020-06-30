#pragma once

#include <ArduinoJson.h>

namespace BurpRedux {
  namespace State {

    class Interface {

      public:

        using Uid = unsigned int;

        virtual const Uid getUid() const = 0;
        virtual void serialize(const JsonObject & serialized) const = 0;

        template <class Derived>
        const Derived * as() const {
          return static_cast<const Derived *>(this);
        }

    };

  }
}
