#pragma once

#include "../Node/Map.hpp"
#include "List/Instance.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {

      template <size_t nodeCount>
      class Branch : public List::Get, public State::Interface {

        public:

          using Index = List::Index;
          using States = List::Instance<nodeCount>;
          using Map = Node::Map<nodeCount>;

          Branch(const Map & map, const States & states) :
            _states(states),
            _map(map)
          {}

          const State::Interface * get(const Index index) const override {
            return _states.get(index);
          }

          void serialize(const JsonObject & serialized) const override {
            for (size_t index = 0; index < nodeCount; index++) {
              auto field = _map[index]->field;
              _states.get(index)->serialize(serialized[field].template to<JsonObject>());
            }
          }

        private:

          const States _states;
          const Map & _map;

      };

    }
  }
}
