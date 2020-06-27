#pragma once

#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Instance : public Interface {

      public:

        Instance(const Id id);
        const State * init(const StateList & list) const override;
        void deserialize(State * current, const JsonObject & serialized) const override;
        const State * reduce(const Id id, const State * previous, const State * next) const override;

      private:

        const Id _id;

    };

  }
}
