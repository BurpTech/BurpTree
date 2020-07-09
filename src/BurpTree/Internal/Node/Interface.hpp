#pragma once

#include "Interface2.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <class State>
      class Interface : public Interface2 {

        public:

          virtual const State * getState() const = 0;

      };

    }
  }
}
