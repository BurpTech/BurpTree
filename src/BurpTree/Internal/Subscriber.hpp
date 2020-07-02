#pragma once

namespace BurpTree {
  namespace Internal {

    template <class State>
    class Subscriber {

      public:

        virtual void setup(const State * initial) = 0;
        virtual void update(const State * next) = 0;

    };

  }
}
