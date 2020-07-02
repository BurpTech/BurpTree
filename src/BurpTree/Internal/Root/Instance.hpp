#pragma once

#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      template <class Node>
      class Instance : public Interface {

        public:

          Instance(Node & node) :
            _node(node),
            _dispatching(false),
            _notifying(false),
            _deserializing(false)
          {}

          void deserialize(const JsonObject & object) override {
            _deserializing = true;
            _node.deserialize(object);
            _deserializing = false;
          }

          void loop() override {
            // Notify asynchronously so that 
            // actions can be batched synchronously.
            // State resolution is always synchronous
            _notifying = true;
            _node.notify();
            _notifying = false;
          }

          const Status & dispatch(const Id id) override {
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
            _node.dispatch(id);
            _dispatching = false;
            return _status;
          }

        private:

          Node & _node;
          bool _dispatching;
          bool _notifying;
          bool _deserializing;
          Status _status;

      };

    }
  }
}
