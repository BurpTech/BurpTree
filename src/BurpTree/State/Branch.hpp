#pragma once

#include <array>
#include <cstddef>
#include "List/Instance.hpp"
#include "Instance.hpp"

namespace BurpTree {
  namespace State {

    template <size_t length>
    class Branch : public List::Get, public State::Instance {

      public:

        using State = List::Get::State;
        using Index = List::Get::Index;
        using States = List::Instance<length>;
        using Fields = std::array<const char *, length>;

        Branch(const Uid uid, const Fields & fields, const States & states) :
          Instance(uid),
          _states(states),
          _fields(fields)
        {}

        const State * get(const Index index) const override {
          return _states.get(index);
        }

        void serialize(const JsonObject & serialized) const override {
          for (size_t index = 0; index < length; index++) {
            auto field = _fields[index];
            _states.get(index)->serialize(serialized[field].template to<JsonObject>());
          }
        }

      private:

        const States _states;
        const Fields & _fields;

    };

  }
}
