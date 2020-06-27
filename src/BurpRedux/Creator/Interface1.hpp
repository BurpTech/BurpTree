#pragma once

namespace BurpRedux {
  namespace Creator {

    template <class State>
    class Interface1 {

      public:

        virtual State * begin(const State * previous) = 0;
        virtual void commit(const State * previous) = 0;

    };

  }
}
