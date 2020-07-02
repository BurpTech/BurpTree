#pragma once

#include "../../Publisher.hpp"
#include "../Interface.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {
      namespace Leaf {

        template <class Factory, size_t subscriberCount>
        class Instance : public Node::Interface, public Interface<Factory> {

          public:

            using State = typename Factory::State;
            using StateInterface = Internal::State::Interface;
            using Publisher = Internal::Publisher<State, subscriberCount>;
            using Subscribers = typename Publisher::Subscribers;
            using Id = Node::Id;

            Instance(const Id id, Factory & factory, const Subscribers subscribers) :
              _id(id),
              _factory(factory),
              _publisher(subscribers),
              _notify(false)
            {}

            const Id getId() const override {
              return _id;
            }

            Factory & getFactory() const override {
              return _factory;
            }

            const StateInterface * deserialize(const JsonObject & serialized) override {
              if (!_factory.deserialize(serialized)) {
                _factory.createDefault();
              }
              auto state = _factory.getState();
              _publisher.setup(state);
              return state;
            }

            const StateInterface * dispatch(const Id id) override {
              if (id == _id) {
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
}
