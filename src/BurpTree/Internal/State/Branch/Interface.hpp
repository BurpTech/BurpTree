#pragma once

#include "../List/Get.hpp"
#include "../Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Branch {

        class Interface : public List::Get, public State::Interface {};

      }
    }
  }
}
