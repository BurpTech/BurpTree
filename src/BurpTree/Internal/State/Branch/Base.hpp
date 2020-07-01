#pragma once

#include "../List/Get.hpp"
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Branch {

        class Base : public List::Get, public State::Base {

          public:

            Base(const Uid uid) :
              State::Base(uid)
            {}

        };

      }
    }
  }
}
