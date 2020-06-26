#pragma once

namespace BurpRedux {
  namespace Creator {

    template <class State, class Params>
    class Interface {

      public:

        virtual const State * init(const Params & params) = 0;
        virtual const State * replace(const State * previous, const Params & params) = 0;

    };

  }
}
