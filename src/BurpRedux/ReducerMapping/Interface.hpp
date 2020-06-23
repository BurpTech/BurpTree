#pragma once

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class CombinedParams, class Action>
    class Interface {

      public:

        virtual bool reduce(const CombinedState * state, CombinedParams & params, const Action & action) = 0;

    };

  }
}
