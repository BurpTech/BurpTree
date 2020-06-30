#include "Constants.hpp"
#include "State.hpp"

namespace BurpTreeTest {

  State::State(const Uid uid, const char * persistent, const int data) :
    BurpTree::State(uid),
    persistent(persistent),
    data(data)
  {}

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void StateFactory::setInitialPersistent(const char * persistent) {
    _persistent = persistent;
  }

  const BurpTree::Status & StateFactory::getStatus() const {
    return _status;
  }

  const BurpTree::State * StateFactory::deserialize(const JsonObject & serialized) {
    return _create([&](const Uid uid, void * address) {
        _status.set(Status::Level::INFO, Status::noError);
        return new(address) BurpTreeTest::State(uid, _persistent, serialized[dataField].as<int>());
    });
  }

  const BurpTree::State * StateFactory::incrementData() {
    return _create([&](const Uid uid, void * address) {
        _status.set(Status::Level::INFO, Status::noError);
        return new(address) BurpTreeTest::State(uid, _previous->persistent, _previous->data + 1);
    });
  }

  const BurpTree::State * StateFactory::setPersistent(const char * persistent) {
    return _create([&](const Uid uid, void * address) {
        _status.set(Status::Level::INFO, Status::noError);
        return new(address) BurpTreeTest::State(uid, persistent, _previous->data);
    });
  }

  const BurpTree::State * StateFactory::setError() {
    return _create([&](const Uid uid, void * address) {
        _status.set(Status::Level::ERROR, Status::error);
        return nullptr;
    });
  }

}
