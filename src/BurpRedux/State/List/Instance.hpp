#pragma once

#include <array>
#include <cstddef>
#include "Interface.hpp"

namespace BurpRedux {
  namespace State {
    namespace List {

      template <size_t length>
      class Instance : public Interface {

        public:

          using States = std::array<State *, length>;

          Instance() {}

          Instance(States states) :
            _states(states)
          {}

          State * get(const Index index) const override {
            return _states[index];
          }

          void set(const Index index, State * state) override {
            _states[index] = state;
          }

        protected:

          States _states;

      };

    }
  }
}
