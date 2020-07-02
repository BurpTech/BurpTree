#pragma once

#include <array>
#include "Subscriber.hpp"

namespace BurpTree {
  namespace Internal {

    template <class State, size_t subscriberCount>
    class Publisher {

      public:

        using Subscribers = std::array<Subscriber<State> *, subscriberCount>;

        Publisher(const Subscribers subscribers) :
            _subscribers(subscribers)
        {}

        void setup(const State * state) {
          for (auto subscriber : _subscribers) {
            if (subscriber) {
              subscriber->setup(state);
            }
          }
        }

        void notify(const State * state) {
          for (auto subscriber : _subscribers) {
            if (subscriber) {
              subscriber->update(state);
            }
          }
        }

      private:

        const Subscribers _subscribers;

    };

  }
}
