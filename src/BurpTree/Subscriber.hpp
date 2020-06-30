#pragma once

#include "State/Interface.hpp"

namespace BurpTree {

  class Subscriber {

    public:

      using State = State::Interface;

      virtual void setup(const State * initial) = 0;
      virtual void onPublish(const State * next) = 0;

  };

}
