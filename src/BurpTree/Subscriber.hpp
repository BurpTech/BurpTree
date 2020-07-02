#pragma once

#include "Internal/Subscriber.hpp"

namespace BurpTree {
  template <class State>
  using Subscriber = Internal::Subscriber<State>;
}
