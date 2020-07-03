#pragma once

#include "../Publisher.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <class _Factory, size_t subscriberCount>
      class Leaf : public Node::Interface {

        public:

          using Factory = _Factory;
          using State = typename Factory::State;
          using StateInterface = Internal::State::Interface;
          using Publisher = Internal::Publisher<State, subscriberCount>;
          using Subscribers = typename Publisher::Subscribers;
          using Id = Node::Id;

          Leaf(const Id id, Factory & factory, const Subscribers & subscribers) :
            _id(id),
            _factory(factory),
            _publisher(subscribers),
            _notify(false)
          {}

          const Id getId() const {
            return _id;
          }

          Factory & getFactory() const {
            return _factory;
          }

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
            if (changed == _id) {
              _notify = true;
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
              _publisher.notify(_factory.getState());
            }
          }

        private:

          const Id _id;
          Factory & _factory;
          Publisher _publisher;
          bool _notify;

      };

    }
  }
}
