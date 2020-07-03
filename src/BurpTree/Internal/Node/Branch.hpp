#pragma once

#include "../State/Branch.hpp"
#include "../State/Factory/Branch/Instance.hpp"
#include "../Publisher.hpp"
#include "Map.hpp"
#include "Base.hpp"
#include <cstddef>

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <size_t nodeCount>
      class BranchFactoryOwner {

        protected:

          using Factory = State::Factory::Branch::Instance<nodeCount>;
          using Map = Node::Map<nodeCount>;

          BranchFactoryOwner(const Map & map) :
            _factoryInstance(map)
          {}

          Factory _factoryInstance;

      };

      template <size_t nodeCount, size_t subscriberCount>
      class Branch : BranchFactoryOwner<nodeCount>, public Base<State::Factory::Branch::Instance<nodeCount>, subscriberCount> {

        public:

          using StateInterface = State::Interface;
          using State = State::Branch<nodeCount>;
          using BranchFactoryOwner = Node::BranchFactoryOwner<nodeCount>;
          using Factory = typename BranchFactoryOwner::Factory;
          using Base = Node::Base<Factory, subscriberCount>;
          using Map = typename BranchFactoryOwner::Map;
          using Entry = Node::Entry;
          using Subscribers = typename Base::Subscribers;
          using Index = typename State::Index;
          using States = typename State::States;
          using Id = Node::Id;

          Branch(const Map & map, const Subscribers & subscribers) :
            BranchFactoryOwner(map),
            Base(this->_factoryInstance, subscribers),
            _map(map),
            _notify(false)
          {}

          const StateInterface * update(const Id changed) override {
            const State * previous = this->_factoryInstance.getState();
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
              this->_factoryInstance.update(states);
              return this->_factoryInstance.getState();
            }
            return nullptr;
          }

          void notify() override {
            if (_notify) {
              _notify = false;
              for (Index index = 0; index < nodeCount; index++) {
                _map[index]->node->notify();
              }
              this->_publisher.notify(this->_factoryInstance.getState());
            }
          }

        private:

          const Map & _map;
          bool _notify;

      };

    }
  }
}
