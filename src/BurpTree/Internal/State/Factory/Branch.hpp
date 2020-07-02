#pragma once

#include "../../Status.hpp"
#include "../../Node/Map.hpp"
#include "../Branch.hpp"
#include "Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <size_t nodeCount>
        class Branch : public Base<State::Branch<nodeCount>, Status> {

          public:

            using Map = Node::Map<nodeCount>;
            using Entry = Node::Entry;
            using State = State::Branch<nodeCount>;
            using States = typename State::States;
            using Index = List::Index;
            using Base = Factory::Base<State, Status>;

            Branch(const Map & map) :
              _map(map)
            {}

            bool create(const States & states) {
              return Base::create([&]() -> const State * {
                  return new(this->getAddress()) State(_map, states);
              });
            }

            bool deserialize(const JsonObject & serialized) override {
              States states;
              for (Index index = 0; index < nodeCount; index++) {
                const Entry & entry = _map[index];
                auto state = entry.node->deserialize(
                    serialized[entry.field].template as<JsonObject>()
                );
                states.set(index, state);
              }
              return create(states);
            }

            void createDefault() override {
              // Should never get here as we never fail Branch State creation
            }

          private:
            
            const Map & _map;

        };

      }
    }
  }
}
