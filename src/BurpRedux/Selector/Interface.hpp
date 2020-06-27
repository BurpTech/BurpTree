#pragma once

#include "../Publisher/Interface.hpp"
#include "../Subscriber/Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    class Interface : public Publisher::Interface, public Subscriber::Interface {};

  }
}
