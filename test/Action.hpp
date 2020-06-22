#pragma once

#include <BurpUnity.hpp>
#include "../src/BurpRedux/Action.hpp"

namespace BurpReduxTest {

  enum class ActionType {
    ACTION
  };
  using Action = BurpRedux::Action<ActionType>;

  struct Payload {
    int data;
  };

  extern Module actionTests;

}
