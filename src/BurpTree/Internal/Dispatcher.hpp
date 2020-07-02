#pragma once

#include <functional>
#include "Root/Interface.hpp"
#include "Node/Id.hpp"

namespace BurpTree {
  namespace Internal {

    template <class Leaf>
    class Dispatcher {

      public:

        using Root = Root::Interface;
        using Id = Node::Id;
        using Factory = typename Leaf::Factory;
        using Status = typename Factory::Status;

        using f_create = std::function<const bool (Factory & factory)>;

        Dispatcher(Root & root, Leaf & leaf) :
          _root(root),
          _factory(leaf.getFactory()),
          _id(leaf.getId())
        {}

        const Internal::Status & dispatch(f_create create) {
          if (create(_factory)) {
            return _root.dispatch(_id);
          }
          return _factory.getStatus();
        }

      private:

        Root & _root;
        Factory & _factory;
        const Id _id;

    };

  }
}
