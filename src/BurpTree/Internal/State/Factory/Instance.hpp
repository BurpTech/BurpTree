#pragma once

#include "Deserializer.hpp"
#include "Pool.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <class State, class Status>
        class Instance : public Deserializer, public Pool<State, Status> {};

      }
    }
  }
}
