#pragma once

#include "Internal/Node/Leaf.hpp"

namespace BurpTree {
  template <size_t subscriberCount>
  using Leaf = Internal::Node::Leaf<subscriberCount>;
}
