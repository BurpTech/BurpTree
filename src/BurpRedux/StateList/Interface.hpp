#pragma once

#include <cstddef>
#include "../State/Interface.hpp"

namespace BurpRedux {
  namespace StateList {

    class Interface {

      public:

        using State = State::Interface;
        using Index = size_t;

        virtual State * get(const Index index) const = 0;

    };

  }
}
