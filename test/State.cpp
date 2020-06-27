#include "Constants.hpp"
#include "State.hpp"

namespace BurpReduxTest {

  State::State(const InitParams & params, const Uid uid) :
    BurpRedux::State::Instance(uid),
    data(0),
    persistent(params.persistent)
  {}

  State::State(const State * previous, const Uid uid) :
    BurpRedux::State::Instance(uid),
    data(0),
    persistent(previous->persistent)
  {}

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void State::deserialize(const JsonObject & serialized) {
    data = serialized[dataField];
  }

}
