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

            virtual void set(const Index index, const Base * state) = 0;

        };

      }
    }
  }
}
