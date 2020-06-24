# pragma once

#include "../src/BurpRedux/State/Interface.hpp"

namespace BurpReduxTest {

  struct Params {
    int data1;
    int data2;
  };

  class State : public BurpRedux::State::Interface {

    public: 

      int data1;
      int data2;

      State(const Params & params) :
        data1(params.data1),
        data2(params.data2)
      {}

      unsigned long getUid() const override {
        return (unsigned long)this;
      }

  };

  struct CombinedParams {
    const State * one;
    const State * two;
    const State * three;
  };

  class CombinedState : public BurpRedux::State::Interface {

    public:

      const BurpReduxTest::State * one;
      const BurpReduxTest::State * two;
      const BurpReduxTest::State * three;

      CombinedState(const CombinedParams & params) :
          one(params.one),
          two(params.two),
          three(params.three)
      {}

      unsigned long getUid() const override {
        return (unsigned long)this;
      }

  };

  template <class State, class Params>
  const State * create(const State * previous, const Params & params) {
    const State * next = new State(params);
    delete previous;
    return next;
  }

}
