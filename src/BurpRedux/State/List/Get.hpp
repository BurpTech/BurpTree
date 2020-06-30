#pragma once

#include <cstddef>
#include "../Interface.hpp"

namespace BurpRedux {
  namespace State {
    namespace List {

      class Get {

        public:

          using Index = size_t;
          using State = State::Interface;

          virtual const State * get(const Index index) const = 0;

      };

    }
  }
}
