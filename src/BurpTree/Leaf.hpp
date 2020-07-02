#pragma once

#include "Internal/Node/Leaf/Instance.hpp"

namespace BurpTree {
  template <class Factory, size_t subscriberCount>
  using Leaf = Internal::Node::Leaf::Instance<Factory, subscriberCount>;
}
