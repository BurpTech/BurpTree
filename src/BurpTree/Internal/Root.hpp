#pragma once

#include "State/Base.hpp"
#include "Node/Interface.hpp"
#include "Status.hpp"

namespace BurpTree {
  namespace Internal {

    class Root {

      public:

        class Status : public Internal::Status {

          public: 

            using Level = Status::Level;

            enum : Status::Code {
              noError,
              dispatchDuringNotification,
              dispatchDuringDeserialize,
              dispatchDuringDispatch
            };

            const char * c_str() const override;

        };

        using State = State::Base;
        using Node = Node::Interface;
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
}
