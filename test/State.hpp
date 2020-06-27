# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/Creator/Instance.hpp"

namespace BurpReduxTest {

  struct InitParams {
    const char * persistent;
  }

  class State : public BurpRedux::State::Instance {

    public: 

      int data;
      const char * persistent;

      State(const InitParams & params, const Uid uid);
      State(const State * previous, const Uid uid);
      void serialize(const JsonObject & serialized) const override;
      void deserialize(const JsonObject & serialized) override;

  };

  using Creator = BurpRedux::Creator::Instance<State, InitParams>;

}
