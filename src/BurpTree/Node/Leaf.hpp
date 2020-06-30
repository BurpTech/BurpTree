#pragma once

#include <array>
#include "../Publisher.hpp"
#include "../State/Factory/Deserializer.hpp"
#include "Interface.hpp"

namespace BurpTree {
  namespace Node {

    template <size_t subscriberCount>
    class Leaf : public Interface {

      public:

        using Deserializer = BurpTree::State::Factory::Deserializer;
        using Publisher = BurpTree::Publisher<subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;
        using State = Interface::State;
        using Id = Interface::Id;

        Leaf(const Id id, Deserializer & deserializer, const Subscribers subscribers) :
          _id(id),
          _deserializer(deserializer),
          _publisher(subscribers)
        {}

        const State * deserialize(const JsonObject & object) override {
          return _deserializer.deserialize(object);
        }

        const State * reduce(const Id id, const State * next) override {
          if (id == _id) {
            return next;
          }
          return _publisher.getState();
        }

        const State * getState() const override {
          return _publisher.getState();
        }

        void setup(const State * initial) override {
          _publisher.setup(initial);
        }

        void onPublish(const State * next) override {
          _publisher.publish(next);
        }

      private:

        const Id _id;
        Deserializer & _deserializer;
        Publisher _publisher;

    };

  }
}
