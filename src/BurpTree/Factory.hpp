#pragma once

#include "Internal/State/Factory/Pool.hpp"
#include "Internal/State/Factory/Deserializer.hpp"

namespace BurpTree {
  template <class State>
  class Factory : public Internal::State::Factory::Deserializer, public Internal::State::Factory::Pool<State> {};
}
