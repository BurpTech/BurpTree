# pragma once

namespace BurpReduxTest {

  class State {

    public: 

      int data1;
      int data2;

      State(int data1, int data2) :
        data1(data1),
        data2(data2)
      {}

      State(const State & state) :
        data1(state.data1),
        data2(state.data2)
      {}

  };

}
