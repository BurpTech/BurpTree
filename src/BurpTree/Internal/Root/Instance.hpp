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
            _updating(false),
            _notifying(false),
            _settingUp(false)
          {}

          bool setup(const JsonVariant & serialized) override {
            _settingUp = true;
            bool result = _node.setup(serialized);
            _settingUp = false;
            return result;
          }

          void loop() override {
            // Notify asynchronously so that 
            // actions can be batched synchronously.
            // State resolution is always synchronous
            _notifying = true;
            _node.notify();
            _notifying = false;
          }

          const Status & update(const Id changed) override {
            if (_settingUp) {
              // prevent dispatch during deserialization and report error
              _status.set(Status::Level::ERROR, Status::updateDuringSetup);
              return _status;
            }
            if (_updating) {
              // prevent dispatch during reduce and report error
              _status.set(Status::Level::ERROR, Status::updateDuringUpdate);
              return _status;
            }
            if (_notifying) {
              // don't prevent dispatch during notification but
              // report warning so that users can detect when it happens
              _status.set(Status::Level::WARNING, Status::updateDuringNotification);
            } else {
              _status.set(Status::Level::INFO, Status::ok);
            }
            _updating = true;
            _node.update(changed);
            _updating = false;
            return _status;
          }

        private:

          Node & _node;
          bool _updating;
          bool _notifying;
          bool _settingUp;
          Status _status;

      };

    }
  }
}
