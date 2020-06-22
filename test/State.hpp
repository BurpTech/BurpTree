# pragma once

#include "../src/BurpRedux/State.hpp"

namespace BurpReduxTest {

  class State : public BurpRedux::State<State> {

    public: 

      int data1;
      int data2;

      State(int data1, int data2) :
        data1(data1),
        data2(data2)
      {}

      unsigned long getUid() const override {
        return (unsigned long)this;
      }

  };

}
