#pragma once

#include "../Subscriber.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input>
    class Subscriber : public Interface, public BurpRedux::Subscriber<Input> {};

  }
}
