#pragma once

#include <ArduinoJson.h>
#include "../Status.hpp"

namespace BurpRedux {
  namespace State {

    class Interface {

      public:

        using Uid = unsigned int;
        using Error = unsigned int;

        static constexpr Error noError = 0;

        virtual const Uid getUid() const = 0;
        virtual const Status & getStatus() const = 0;
        virtual void serialize(const JsonObject & serialized) const = 0;
        virtual void deserialize(const JsonObject & serialized) = 0;

        template <class Derived>
        Derived * as() {
          return static_cast<Derived *>(this);
        }

    };

  }
}
