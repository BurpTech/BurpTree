#pragma once

#include <array>
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      struct Entry {
        const char * field;
        Interface * node;
      };

      template <size_t nodeCount>
      using Map = std::array<const Entry, nodeCount>;

    }
  }
}
