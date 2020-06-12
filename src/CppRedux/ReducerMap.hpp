#pragma once

#include <functional>
#include "Action.hpp"

namespace CppRedux {

  template <class CombinedState, class Action>
  class ReducerMap {

    public:

      using f_update = std::function<const CombinedState * (const CombinedState & state)>;

      virtual const CombinedState * init(const CombinedState * previous, const CombinedState & initialState, f_update update) const = 0;
      virtual const CombinedState * reduce(const CombinedState * previous, const Action & action, f_update update) const = 0;

  };

}
