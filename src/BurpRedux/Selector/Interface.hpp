#pragma once

#include "../Publisher/Interface.hpp"
#include "../Subscriber/Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input, class Output>
    class Interface : public Publisher::Interface<Output>, public Subscriber::Interface<Input> {};

  }
}
