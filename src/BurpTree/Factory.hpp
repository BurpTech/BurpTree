#pragma once

#include "Internal/State/Factory/Instance.hpp"

namespace BurpTree {
  template <class State, class Status>
  using Factory = Internal::State::Factory::Instance<State, Status>;
}
