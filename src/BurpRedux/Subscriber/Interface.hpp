#pragma once

namespace BurpRedux {
  namespace Subscriber {

    template <class State>
    class Interface {

      public:

        virtual void setup(State * initial) = 0;
        virtual void onPublish(State * next) = 0;

    };

  }
}
