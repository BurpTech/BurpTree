#include "State.hpp"

namespace CppRedux {

  State::State(const State * state) :
    reduxSequence(state ? state->reduxSequence + 1 : 0)
  {}

}
