#pragma once

#include <BddUnity.hpp>
#include "../src/CppRedux/Action.hpp"

namespace CppReduxTest {
  namespace Action {

    enum class ActionType {
      ACTION
    };
    using Action = CppRedux::Action<ActionType>;

    struct Payload {
      int data;
    };

    extern Module tests;

  }
}
