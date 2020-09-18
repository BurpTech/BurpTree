#pragma once

#include "../../../Node/Map.hpp"
#include "../../Branch.hpp"
#include "../Base.hpp"
#include "Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {
        namespace Branch {

          template <size_t nodeCount>
          class Instance : public Base<State::Branch<nodeCount>, Status> {

            public:

              using Map = Node::Map<nodeCount>;
              using Entry = Node::Entry;
              using State = State::Branch<nodeCount>;
              using States = typename State::States;
              using Index = List::Index;
              using Base = Factory::Base<State, Status>;

              Instance(const Map & map) :
                _map(map)
              {}

              bool update(const States & states) {
                return this->create([&]() -> const State * {
                    return this->ok(new(this->getAddress()) State(_map, states));
                });
              }

              bool deserialize(const JsonVariant & serialized) override {
                return this->create([&]() -> const State * {
                    States states;
                    for (Index index = 0; index < nodeCount; index++) {
                      const Entry * entry = _map[index];
                      auto state = entry->node->setup(
                          serialized[entry->field].template as<JsonVariant>()
                      );
                      if (!state) {
                        return this->error(Status::setupFailed);
                      }
                      states.set(index, state);
                    }
                    return this->ok(new(this->getAddress()) State(_map, states));
                });
              }

              bool createDefault() override {
                return this->create([&]() -> const State * {
                    return this->error(Status::noDefault);
                });
              }

            private:
              
              const Map & _map;

          };

        }
      }
    }
  }
}
