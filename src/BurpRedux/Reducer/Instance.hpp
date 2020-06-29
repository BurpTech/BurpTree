#pragma once

#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    class Instance : public Interface {

      public:

        Instance(const Id id);
        State * init(const StateList & list) override;
        State * reduce(const Id id, State * previous, State * next) override;

      private:

        const Id _id;

    };

  }
}
