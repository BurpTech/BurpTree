#pragma once

namespace CppRedux {

  class State {

    public:

      const unsigned long reduxSequence;

      State(const State * state);

  };

}
