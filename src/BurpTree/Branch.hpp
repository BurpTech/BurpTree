#pragma once

#include "Internal/Node/Branch.hpp"

namespace BurpTree {
  template <size_t nodeCount, size_t subscriberCount>
  using Branch = Internal::Node::Branch<nodeCount, subscriberCount>;
}
