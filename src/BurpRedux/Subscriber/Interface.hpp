#pragma once

namespace BurpRedux {
  namespace Subscriber {

    template <class State>
    class Interface {

      public:

        virtual void onPublish(const State * state) = 0;

    };

  }
}
