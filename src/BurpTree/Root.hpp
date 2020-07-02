#pragma once

#include "Internal/Root/Instance.hpp"

namespace BurpTree {
  template <class Node>
  using Root = Internal::Root::Instance<Node>;
}
