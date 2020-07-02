#pragma once

#include "Internal/Node/Leaf.hpp"

namespace BurpTree {
  template <class Factory, size_t subscriberCount>
  using Leaf = Internal::Node::Leaf<Factory, subscriberCount>;
}
