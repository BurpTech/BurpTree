#include "State.hpp"

namespace CppRedux {

  State::State(const State * state) :
    cppReduxSequence(state ? state->cppReduxSequence + 1 : 0)
  {}

}
