#pragma once

#include <BurpUnity.hpp>
#include "../src/BurpRedux/Action.hpp"

namespace BurpReduxTest {

  enum class ActionType {
    ACTION,
    ACTION_1,
    ACTION_2,
    ACTION_3
  };
  using Action = BurpRedux::Action<ActionType>;

  struct Payload {
    int data;
  };

  extern Module actionTests;

}
