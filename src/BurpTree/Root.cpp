#include "Root.hpp"

namespace BurpTree {

  const char * Root::Status::c_str() const {
    switch (getCode()) {
      case noError:
        return "BurpTree::Root : no error";
      case dispatchDuringNotification:
        return "BurpTree::Root : dispatch during notification";
      case dispatchDuringDeserialize:
        return "BurpTree::Root : dispatch during deserialize";
      case dispatchDuringDispatch:
        return "BurpTree::Root : dispatch during dispatch";
      default:
        return "BurpTree::Root : unknown";
    }
  }

  Root::Root(Node & node) :
    _node(node),
    _dispatching(false),
    _notifying(false),
    _deserializing(false),
    _next(nullptr)
  {}

  void Root::deserialize(const JsonObject & object) {
    _deserializing = true;
    const State * initial = _node.deserialize(object);
    _node.setup(initial);
    _deserializing = false;
  }

  void Root::loop() {
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

  const Root::Status & Root::dispatch(const Id id, const State * next) {
    if (_deserializing) {
      // prevent dispatch during deserialization and report error
      _status.set(Status::Level::ERROR, Status::dispatchDuringDeserialize);
      return _status;
    }
    if (_dispatching) {
      // prevent dispatch during reduce and report error
      _status.set(Status::Level::ERROR, Status::dispatchDuringDispatch);
      return _status;
    }
    _status.set(Status::Level::INFO, Status::noError);
    if (_notifying) {
      // don't prevent dispatch during notification but
      // report warning so that users can detect when it happens
      _status.set(Status::Level::WARNING, Status::dispatchDuringNotification);
    }
    _dispatching = true;
    _next = _node.dispatch(id, next);
    _dispatching = false;
    return _status;
  }

  const Root::State * Root::getState() const {
    return _node.getState();
  }

}
