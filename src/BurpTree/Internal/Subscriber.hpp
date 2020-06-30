#pragma once

#include "State/Base.hpp"

namespace BurpTree {
  namespace Internal {

    class Subscriber {

      public:

        using State = State::Base;

        virtual void setup(const State * initial) = 0;
        virtual void onPublish(const State * next) = 0;

    };

  }
}
