#pragma once

#include <functional>
#include "Root.hpp"
#include "Node/Interface.hpp"
#include "Status.hpp"

namespace BurpTree {
  namespace Internal {

    template <class Factory>
    class Dispatcher {

      public:

        using Id = Internal::Node::Interface::Id;

        using f_create = std::function<const State::Base * (Factory & factory)>;

        Dispatcher(Root & root, Id id, Factory & factory) :
          _root(root),
          _id(id),
          _factory(factory)
        {}

        const Status & dispatch(f_create create) {
          const State::Base * state = create(_factory);
          if (state) {
            return _root.dispatch(_id, state);
          }
          return _factory.getStatus();
        }

      private:

        Root & _root;
        Id _id;
        Factory & _factory;

    };

  }
}
