#pragma once

#include <array>
#include "Interface2.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      struct Entry {
        const char * field;
        Interface2 * node;
      };

      template <size_t nodeCount>
      using Map = std::array<const Entry *, nodeCount>;

    }
  }
}
