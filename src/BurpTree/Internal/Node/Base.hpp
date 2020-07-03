#pragma once

#include "../Publisher.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <class _Factory, size_t subscriberCount>
      class Base : public Interface {

        public:

          using StateInterface = State::Interface;
          using Factory = _Factory;
          using State = typename Factory::State;
          using Publisher = Internal::Publisher<State, subscriberCount>;
          using Subscribers = typename Publisher::Subscribers;

          Base(Factory & factory, const Subscribers & subscribers) :
            _factory(factory),
            _publisher(subscribers)
          {}

          Factory & getFactory() const {
            return _factory;
          }

          const State * getState() const {
            return _factory.getState();
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

        protected:

          Factory & _factory;
          Publisher _publisher;

      };

    }
  }
}
