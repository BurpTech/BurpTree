#pragma once

#include "../Action/Interface.hpp"

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class CombinedParams>
    class Interface {

      public:

        virtual bool reduce(const CombinedState * state, CombinedParams & params, const Action::Interface & action) = 0;

    };

  }
}
