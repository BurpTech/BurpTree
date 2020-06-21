#pragma once

#include <functional>

namespace BurpRedux {
  template <class State, class Action>
  using f_reducer = std::function<const State * (const State * state, const Action & action)>;
}
