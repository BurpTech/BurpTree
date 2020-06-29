#pragma once

#include <cstddef>
#include "../Interface.hpp"

namespace BurpRedux {
  namespace State {
    namespace List {

      class Interface {

        public:

          using State = State::Interface;
          using Index = size_t;

          virtual State * get(const Index index) const = 0;
          virtual void set(const Index index, State * state) = 0;

      };

    }
  }
}
