# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/Creator/Instance.hpp"

namespace BurpReduxTest {

  struct Params {
    int data1;
    int data2;
  };

  class State : public BurpRedux::State::Instance {

    public: 

      int data1;
      int data2;

      State(const Params & params, const unsigned long uid) :
        BurpRedux::State::Instance(uid),
        data1(params.data1),
        data2(params.data2)
      {}

  };

  struct CombinedParams {
    const State * one;
    const State * two;
    const State * three;
  };

  class CombinedState : public BurpRedux::State::Instance {

    public:

      const BurpReduxTest::State * one;
      const BurpReduxTest::State * two;
      const BurpReduxTest::State * three;

      CombinedState(const CombinedParams & params, const unsigned long uid) :
          BurpRedux::State::Instance(uid),
          one(params.one),
          two(params.two),
          three(params.three)
      {}

  };

  using Creator = BurpRedux::Creator::Instance<State, Params>;
  using CombinedCreator = BurpRedux::Creator::Instance<CombinedState, CombinedParams>;

}
