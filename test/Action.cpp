#include "Action.hpp"

namespace BurpReduxTest {
  namespace Action {

    void incrementData(const State * previous, State * next) {
      next->data = previous->data + 1;
    }

    void setExternal(const State * previous, const char * external, State * next) {
      next->data = previous->data;
      next->external = external;
    }

    void setError(State * next) {
      next->setError(1);
    }

  }
}
