#pragma once

#include "../State/Branch.hpp"
#include "../State/Factory/Branch/Instance.hpp"
#include "../Publisher.hpp"
#include "Map.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <size_t nodeCount, size_t subscriberCount>
      class Branch : public Interface {

        public:

          using Map = Node::Map<nodeCount>;
          using Entry = Node::Entry;
          using StateInterface = Internal::State::Interface;
          using State = State::Branch<nodeCount>;
          using Publisher = Internal::Publisher<State, subscriberCount>;
          using Subscribers = typename Publisher::Subscribers;
          using Factory = Internal::State::Factory::Branch::Instance<nodeCount>;
          using Index = typename State::Index;
          using States = typename State::States;
          using Id = Node::Id;

          Branch(const Map & map, const Subscribers & subscribers) :
            _map(map),
            _factory(map),
            _publisher(subscribers),
            _notify(false)
          {}

          const StateInterface * setup(const JsonObject & serialized) override {
            if (!_factory.deserialize(serialized)) {
              if (!_factory.createDefault()) {
                return nullptr;
              }
            }
            auto state = _factory.getState();
            _publisher.setup(state);
            return state;
          }

          const StateInterface * update(const Id changed) override {
            const State * previous = _factory.getState();
            States states;
            for (Index index = 0; index < nodeCount; index++) {
              auto state = _map[index]->node->update(changed);
              if (state) {
                _notify = true;
                states.set(index, state);
              } else {
                states.set(index, previous->get(index));
              }
            }
            if (_notify) {
              _factory.update(states);
              return _factory.getState();
            }
            return nullptr;
          }

          const State * getState() const {
            return _factory.getState();
          }

          void notify() override {
            if (_notify) {
              _notify = false;
              for (Index index = 0; index < nodeCount; index++) {
                _map[index]->node->notify();
              }
              _publisher.notify(_factory.getState());
            }
          }

        private:

          const Map & _map;
          Factory _factory;
          Publisher _publisher;
          bool _notify;

      };

    }
  }
}
