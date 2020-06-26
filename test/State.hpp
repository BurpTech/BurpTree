# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/Creator/Instance.hpp"

namespace BurpReduxTest {

  constexpr char data1Field[] = "data1";
  constexpr char data2Field[] = "data2";

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

      void serialize(const JsonObject & serialized) const override {
        serialized[data1Field] = data1;
        serialized[data2Field] = data2;
      }

  };

  void deserialize(const JsonObject & serialized, Params & params);

  constexpr char oneField[] = "one";
  constexpr char twoField[] = "two";
  constexpr char threeField[] = "three";

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

      void serialize(const JsonObject & serialized) const override {
        one->serialize(serialized[oneField].to<JsonObject>());
        two->serialize(serialized[twoField].to<JsonObject>());
        three->serialize(serialized[threeField].to<JsonObject>());
      }

  };

  using Creator = BurpRedux::Creator::Instance<State, Params>;
  using CombinedCreator = BurpRedux::Creator::Instance<CombinedState, CombinedParams>;

}
