#pragma once

namespace BurpRedux {
  namespace Creator {

    template <class State, class Params>
    class Interface {

      public:

        virtual const State * create(const State * previous, const Params & params) = 0;

    };

  }
}
