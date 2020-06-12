#pragma once

#include <BddUnity.hpp>
#include "../src/CppRedux/State.hpp"

namespace CppReduxTest {
  namespace State {

    class State : public CppRedux::State {

      public:

        const char * data;

        State(const State * previous, const char * data) :
          CppRedux::State(previous),
          data(data)
        {}

        State(const State * previous, const State & next) :
          State(previous, next.data)
        {}

        State(const char * data) :
          State(nullptr, data)
        {}

    };

    extern Module tests;

  }
}
