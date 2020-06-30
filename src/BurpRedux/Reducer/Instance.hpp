#pragma once

#include "../State/Factory/Deserializer.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Instance : public Interface {

      public:

        using Deserializer = BurpRedux::State::Factory::Deserializer;

        Instance(const Id id, Deserializer & deserializer);
        const State * deserialize(const JsonObject & object) override;
        const State * reduce(const Id id, const State * previous, const State * next) override;

      private:

        const Id _id;
        Deserializer & _deserializer;

    };

  }
}
