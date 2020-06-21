#pragma once

#include "../Publisher/Interface.hpp"
#include "../Subscriber.hpp"

namespace CppRedux {
  namespace Selector {

    template <class Input, class Output>
    class Interface : public Publisher::Interface<Output>, public Subscriber<Input> {};

  }
}
