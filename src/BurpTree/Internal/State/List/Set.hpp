#pragma once

#include <cstddef>
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace List {

        class Set {

          public:

            using Index = size_t;
            using State = Base;

            virtual void set(const Index index, const State * state) = 0;

        };

      }
    }
  }
}
