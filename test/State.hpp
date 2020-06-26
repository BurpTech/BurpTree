# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/Creator/Instance.hpp"

namespace BurpReduxTest {

  constexpr char data1Field[] = "data1";

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
      }

  };

  void deserialize(const JsonObject & serialized, Params & params);

  constexpr char oneField[] = "one";
  constexpr char twoField[] = "two";
  constexpr char threeField[] = "three";

  struct CombinedParams {
    Params one;
    Params two;
    Params three;
  };

  class CombinedState : public BurpRedux::State::Instance {

    public:

      const BurpReduxTest::State * one;
      const BurpReduxTest::State * two;
      const BurpReduxTest::State * three;

      CombinedState() :
          BurpRedux::State::Instance(0),
          one(nullptr),
          two(nullptr),
          three(nullptr)
      {}

      CombinedState(const CombinedState & state, const unsigned long uid) :
          BurpRedux::State::Instance(uid),
          one(state.one),
          two(state.two),
          three(state.three)
      {}

      void serialize(const JsonObject & serialized) const override {
        one->serialize(serialized[oneField].to<JsonObject>());
        two->serialize(serialized[twoField].to<JsonObject>());
        three->serialize(serialized[threeField].to<JsonObject>());
      }

  };

  using Creator = BurpRedux::Creator::Instance<State, Params>;
  using CombinedCreator = BurpRedux::Creator::Instance<CombinedState, CombinedState>;

}
