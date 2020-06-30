#include "Store.hpp"

namespace BurpTree {

  const char * Store::Status::c_str() const {
    switch (getCode()) {
      case noError:
        return "BurpTree::Store : no error";
      case dispatchDuringNotification:
        return "BurpTree::Store : dispatch during notification";
      case dispatchDuringDeserialize:
        return "BurpTree::Store : dispatch during deserialize";
      case dispatchDuringReduce:
        return "BurpTree::Store : dispatch during reduce";
      default:
        return "BurpTree::Store : unknown";
    }
  }

  Store::Store(Node & node) :
    _node(node),
    _reducing(false),
    _notifying(false),
    _deserializing(false),
    _next(nullptr)
  {}

  void Store::deserialize(const JsonObject & object) {
    _deserializing = true;
    const State * initial = _node.deserialize(object);
    _node.setup(initial);
    _deserializing = false;
  }

  void Store::loop() {
    // Notify asynchronously so that 
    // actions can be batched synchronously.
    // State reduction is always synchronous
    if (_next) {
      const State * state = _next;
      _next = nullptr;
      _notifying = true;
      _node.onPublish(state);
      _notifying = false;
    }
  }

  const Store::Status & Store::dispatch(const Id id, const State * next) {
    if (_deserializing) {
      // prevent dispatch during deserialization and report error
      _status.set(Status::Level::ERROR, Status::dispatchDuringDeserialize);
      return _status;
    }
    if (_reducing) {
      // prevent dispatch during reduce and report error
      _status.set(Status::Level::ERROR, Status::dispatchDuringReduce);
      return _status;
    }
    _status.set(Status::Level::INFO, Status::noError);
    if (_notifying) {
      // don't prevent dispatch during notification but
      // report warning so that users can detect when it happens
      _status.set(Status::Level::WARNING, Status::dispatchDuringNotification);
    }
    _reducing = true;
    _next = _node.reduce(id, next);
    _reducing = false;
    return _status;
  }

  const Store::State * Store::getState() const {
    return _node.getState();
  }

}
