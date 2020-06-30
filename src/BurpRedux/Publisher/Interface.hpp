#pragma once

namespace BurpRedux {
  namespace Publisher {

    template <class State>
    class Interface {

      public:

        virtual const State * getState() const = 0;
        virtual void publish(const State * state) = 0;

    };

  }
}
