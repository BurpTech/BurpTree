# pragma once

namespace BurpReduxTest {

  struct Params {
    int data1;
    int data2;
  };

  class State {

    public: 

      int data1;
      int data2;

      State(const Params * params) :
        data1(params->data1),
        data2(params->data2)
      {}

  };

}
