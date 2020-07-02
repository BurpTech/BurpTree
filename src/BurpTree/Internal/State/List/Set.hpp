#pragma once

#include "Index.hpp"
#include "../Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace List {

        class Set {

          public:

            virtual void set(const Index index, const Interface * state) = 0;

        };

      }
    }
  }
}
