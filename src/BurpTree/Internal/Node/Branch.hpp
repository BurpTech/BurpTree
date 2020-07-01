#pragma once

#include <array>
#include "../State/Branch/Instance.hpp"
#include "../State/Factory/Branch.hpp"
#include "../Publisher.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <size_t nodeCount, size_t subscriberCount>
      class Branch : public Interface {

        public:

          using Node = Interface;

          struct Entry {
            const char * field;
            Node * node;
          };

          using Map = std::array<const Entry, nodeCount>;
          using BranchState = Internal::State::Branch::Instance<nodeCount>;
          using Publisher = Internal::Publisher<subscriberCount>;
          using Subscribers = typename Publisher::Subscribers;
          using Factory = Internal::State::Factory::Branch<nodeCount>;
          using Uid = State::Uid;
          using Uids = std::array<Uid, nodeCount>;
          using Index = typename BranchState::Index;
          using States = typename BranchState::States;
          using Fields = typename Factory::Fields;
          using Nodes = std::array<Node *, nodeCount>;
          using Id = Node::Id;

          Branch(const Map & map, const Subscribers & subscribers) :
            _lists(transposeMap(map)),
            _factory(_lists.fields),
            _publisher(subscribers)
          {}

          const State * deserialize(const JsonObject & object) override {
            States states;
            for (Index index = 0; index < nodeCount; index++) {
              auto state = _lists.nodes[index]->deserialize(
                  object[_lists.fields[index]].template as<JsonObject>()
              );
              _uids[index] = state->getUid();
              states.set(index, state);
            }
            return _factory.create(states);
          }

          const State * dispatch(const Id id, const State * next) override {
            States states;
            bool changed = false;
            for (Index index = 0; index < nodeCount; index++) {
              auto state = _lists.nodes[index]->dispatch(id, next);
              const Uid uid = state->getUid();
              if (uid != _uids[index]) {
                changed = true;
              }
              states.set(index, state);
              _uids[index] = uid;
            }
            if (changed) {
              return _factory.create(states);
            }
            return _publisher.getState();
          }

          const State * getState() const override {
            return _publisher.getState();
          }

          void setup(const State * initial) override {
            const BranchState * branch = initial->as<BranchState>();
            for (Index index = 0; index < nodeCount; index++) {
              _lists.nodes[index]->setup(branch->get(index));
            }
            _publisher.setup(initial);
          }

          void onPublish(const State * next) override {
            const BranchState * branch = next->as<BranchState>();
            for (Index index = 0; index < nodeCount; index++) {
              _lists.nodes[index]->onPublish(branch->get(index));
            }
            _publisher.publish(next);
          }

        private:

          struct _Lists {
            Fields fields;
            Nodes nodes;
          };

          const _Lists _lists;
          Factory _factory;
          Uids _uids;
          Publisher _publisher;

          static const _Lists transposeMap(const Map & map) {
            _Lists lists;
            for (Index index = 0; index < nodeCount; index++) {
              const Entry entry = map[index];
              lists.fields[index] = entry.field;
              lists.nodes[index] = entry.node;
            }
            return lists;
          }

      };

    }
  }
}
