#pragma once

#include "Internal/Dispatcher.hpp"

namespace BurpTree {
  template <class Factory>
  using Dispatcher = Internal::Dispatcher<Factory>;
}
