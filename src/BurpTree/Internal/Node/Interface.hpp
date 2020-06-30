#pragma once

#include <ArduinoJson.h>
#include "../State/Base.hpp"
#include "../Subscriber.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      class Interface : public Subscriber {

        public:

          using Id = unsigned int;
          using State = Internal::State::Base;

          virtual const State * deserialize(const JsonObject & object) = 0;
          virtual const State * dispatch(const Id id, const State * next) = 0;
          virtual const State * getState() const = 0;

      };

    }
  }
}
