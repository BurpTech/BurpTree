#pragma once

#include <array>
#include "Get.hpp"
#include "Set.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace List {

        template <size_t length>
        class Instance : public Get, public Set {

          public:

            using Index = Get::Index;
            using States = std::array<const Base *, length>;

            const Base * get(const Index index) const override {
              return _states[index];
            }

            void set(const Index index, const Base * state) override {
              _states[index] = state;
            }

          private:

            States _states;

        };

      }
    }
  }
}
