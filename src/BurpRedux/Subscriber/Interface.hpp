#pragma once

namespace BurpRedux {
  namespace Subscriber {

    template <class State>
    class Interface {

      public:

        virtual void setup(const State * initial) = 0;
        virtual void onPublish(const State * next) = 0;

    };

  }
}
