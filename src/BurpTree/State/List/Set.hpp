#pragma once

#include <cstddef>
#include "../Interface.hpp"

namespace BurpTree {
  namespace State {
    namespace List {

      class Set {

        public:

          using Index = size_t;
          using State = State::Interface;

          virtual void set(const Index index, const State * state) = 0;

      };

    }
  }
}
