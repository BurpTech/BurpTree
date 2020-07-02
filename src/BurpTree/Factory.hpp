#pragma once

#include "Internal/State/Factory/Base.hpp"

namespace BurpTree {
  template <class State, class Status>
  using Factory = Internal::State::Factory::Base<State, Status>;
}
