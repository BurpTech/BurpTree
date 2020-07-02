#pragma once

#include "Index.hpp"
#include "../Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace List {

        class Get {

          public:

            virtual const Interface * get(const Index index) const = 0;

        };

      }
    }
  }
}
