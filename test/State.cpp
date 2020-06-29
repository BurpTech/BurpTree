#include "Constants.hpp"
#include "State.hpp"

namespace BurpReduxTest {

  State::State(const char * persistent, const Uid uid) :
    BurpRedux::State::Instance(uid),
    data(0),
    persistent(persistent)
  {}

  State::State(const State * previous, const Uid uid) :
    BurpRedux::State::Instance(uid),
    data(previous->data),
    persistent(previous->persistent)
  {}

  const Status & State::getStatus() const {
    return _status;
  }

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void State::deserialize(const JsonObject & serialized) {
    data = serialized[dataField];
  }

  void State::incrementData(const State * previous) {
    data = previous->data + 1;
  }

  void State::setPersistent(const State * previous, const char * newPersistent) {
    persistent = newPersistent;
  }

  void State::setError(const State * previous) {
    _status.set(Status::Level::ERROR, Status::error);
  }

}
