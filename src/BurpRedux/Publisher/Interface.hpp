#pragma once

namespace BurpRedux {
  namespace Publisher {

    template <class State>
    class Interface {

      public:

        virtual State * getState() const = 0;
        virtual void publish(State * state) = 0;

    };

  }
}
