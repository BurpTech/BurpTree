#pragma once

#include <ArduinoJson.h>

namespace BurpTree {
  namespace Internal {
    namespace State {

      class Base {

        public:

          using Uid = unsigned int;

          Base(const Uid uid);
          const Uid getUid() const;

          virtual void serialize(const JsonObject & serialized) const = 0;

          template <class Derived>
          const Derived * as() const {
            return static_cast<const Derived *>(this);
          }

        private:

          const Uid _uid;

      };

    }
  }
}
