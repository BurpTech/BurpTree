#pragma once

#include "State/Base.hpp"

namespace BurpTree {
  namespace Internal {

    class Subscriber {

      public:

        virtual void setup(const State::Base * initial) = 0;
        virtual void onPublish(const State::Base * next) = 0;

    };

  }
}
