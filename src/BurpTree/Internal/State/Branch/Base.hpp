#pragma once

#include "../List/Get.hpp"
#include "../Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Branch {

        class Base : public List::Get, public State::Base {

          public:

            using State = Internal::State::Base;

            Base(const Uid uid) :
              State(uid)
            {}

        };

      }
    }
  }
}
