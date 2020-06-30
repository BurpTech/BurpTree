#include "Constants.hpp"
#include "State.hpp"

namespace BurpTreeTest {

  State::State(const Uid uid, const char * persistent, const int data) :
    BurpTree::State::Instance(uid),
    persistent(persistent),
    data(data)
  {}

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void StateFactory::setInitialPersistent(const char * persistent) {
    _persistent = persistent;
  }

  const Status & StateFactory::getStatus() const {
    return _status;
  }

  const BurpTree::State::Interface * StateFactory::deserialize(const JsonObject & serialized) {
    return _create([&](const Uid uid, void * address) {
        return new(address) State(uid, _persistent, serialized[dataField].as<int>());
    });
  }

  const BurpTree::State::Interface * StateFactory::incrementData() {
    return _create([&](const Uid uid, void * address) {
        return new(address) State(uid, _previous->persistent, _previous->data + 1);
    });
  }

  const BurpTree::State::Interface * StateFactory::setPersistent(const char * persistent) {
    return _create([&](const Uid uid, void * address) {
        return new(address) State(uid, persistent, _previous->data);
    });
  }

  const BurpTree::State::Interface * StateFactory::setError() {
    _status.set(Status::Level::ERROR, Status::error);
    return nullptr;
  }

}
