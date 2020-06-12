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
      const char * data;
    };

    extern Module tests;

  }
}
