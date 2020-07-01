#pragma once

#include <cstddef>
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace List {

        class Get {

          public:

            using Index = size_t;

            virtual const Base * get(const Index index) const = 0;

        };

      }
    }
  }
}
