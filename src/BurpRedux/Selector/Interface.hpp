#pragma once

#include "../State/List/Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    class Interface {

      public:

        using Index = State::List::Interface::Index;
        virtual void init(const Index index) = 0;

    };

  }
}
