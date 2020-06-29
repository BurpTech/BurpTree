#pragma once

#include <array>
#include <cstddef>
#include "../../Selector/Interface.hpp"
#include "../Interface.hpp"

namespace BurpRedux {
  namespace Reducer {
    namespace Combined {
      namespace Map {

        using Reducer = Reducer::Interface;
        using Selector = Selector::Interface;

        struct Entry {
          const char * field;
          Reducer * reducer;
          Selector * selector;
        };

        template <size_t length>
        using Instance = std::array<Entry, length>;

      }
    }
  }
}
