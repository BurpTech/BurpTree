#pragma once

#include <array>
#include <cstddef>
#include "Interface.hpp"

namespace BurpRedux {
  namespace StateList {

    template <size_t length>
    class Instance : public Interface {

      public:

        using States = std::array<State *, length>;

        Instance(const States states) :
          _states(states)
        {}

        State * get(const Index index) const override {
          return _states[index];
        }

      private:

        const States _states;

    };

  }
}
