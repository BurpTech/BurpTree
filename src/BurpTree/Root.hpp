#pragma once

#include "State/Interface.hpp"
#include "Node/Interface.hpp"
#include "Status.hpp"

namespace BurpTree {

  class Root {

    public:

      class Status : public BurpTree::Status {

        public: 

          using Level = BurpTree::Status::Level;

          enum Code : BurpTree::Status::Code {
            noError,
            dispatchDuringNotification,
            dispatchDuringDeserialize,
            dispatchDuringDispatch
          };

          const char * c_str() const override;

      };

      using State = BurpTree::State::Interface;
      using Node = BurpTree::Node::Interface;
      using Id = Node::Id;

      Root(Node & node);
      void deserialize(const JsonObject & object);
      void loop();
      const Status & dispatch(const Id id, const State * next);
      const State * getState() const;

    private:

      Node & _node;
      bool _dispatching;
      bool _notifying;
      bool _deserializing;
      const State * _next;
      Status _status;

  };

}
