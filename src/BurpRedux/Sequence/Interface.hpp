#pragma once

#include <memory>

namespace BurpRedux {
  namespace Sequence {

    template <class State>
    class Interface {

      public:

        virtual const State * getState() const = 0;
        virtual unsigned long getId() const = 0;

    };

  }
}
