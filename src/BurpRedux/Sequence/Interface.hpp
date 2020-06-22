#pragma once

#include <memory>

namespace BurpRedux {
  namespace Sequence {

    template <class State>
    class Interface {

      public:

        virtual void assign(const State & state) = 0;
        virtual const State * getState() const = 0;
        virtual unsigned long getId() const = 0;

    };

  }
}
