#pragma once

#include "../Id.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {
      namespace Leaf {

        template <class Factory>
        class Interface {

          public:

            virtual const Id getId() const = 0;
            virtual Factory & getFactory() const = 0;

        };

      }
    }
  }
}
