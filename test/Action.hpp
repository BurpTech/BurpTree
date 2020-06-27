#pragma once

#include "State.hpp"
#include <functional>

namespace BurpReduxTest {
  namespace Action {

    void incrementData(const State * previous, State * next);
    void setExternal(const State * previous, State * next, const char * external);
    void setError(const State * previous, State * next);

  }
}
