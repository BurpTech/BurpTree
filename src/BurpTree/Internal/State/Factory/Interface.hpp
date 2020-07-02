#pragma once

#include <ArduinoJson.h>

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <class State>
        class Interface {

          public:

            virtual const State * getState() const = 0;
            virtual bool deserialize(const JsonObject & serialized) = 0;
            virtual void createDefault() = 0;

        };

      }
    }
  }
}
