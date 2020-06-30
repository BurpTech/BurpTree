#pragma once

#include "../State/Combined.hpp"

namespace BurpRedux {
  namespace Selector {

    class Interface {

      public:

        virtual void init(const size_t index) = 0;

    };

  }
}
