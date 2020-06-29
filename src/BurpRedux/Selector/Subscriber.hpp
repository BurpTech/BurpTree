#pragma once

#include "../Subscriber/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input>
    class Subscriber : public Interface, public BurpRedux::Subscriber::Interface<Input> {};

  }
}
