#pragma once

#include "../State/Interface.hpp"

namespace BurpRedux {
  namespace Publisher {

    class Interface {

      public:

        using State = State::Interface;

        virtual const State * getState() const = 0;
        virtual void publish(const State * state) = 0;

    };

  }
}
