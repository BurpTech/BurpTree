#pragma once

#include "Internal/Updater.hpp"

namespace BurpTree {
  template <class Leaf>
  using Updater = Internal::Updater<Leaf>;
}
