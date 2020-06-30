#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"
#include "../Subscriber.hpp"

namespace BurpTree {
  namespace Node {

    class Interface : public Subscriber {

      public:

        using Id = unsigned int;
        using State = BurpTree::State::Interface;

        virtual const State * deserialize(const JsonObject & object) = 0;
        virtual const State * reduce(const Id id, const State * next) = 0;
        virtual const State * getState() const = 0;

    };

  }
}
