#pragma once

#include <array>
#include "State/Base.hpp"
#include "Subscriber.hpp"

namespace BurpTree {
  namespace Internal {

    template <size_t subscriberCount>
    class Publisher {

      public:

        using State = State::Base;
        using Subscribers = std::array<Subscriber *, subscriberCount>;

        Publisher(const Subscribers subscribers) :
            _subscribers(subscribers),
            _state(nullptr),
            _uid(0)
        {}

        void setup(const State * state) {
          _setState(state, true);
          for (auto subscriber : _subscribers) {
            if (subscriber) {
              subscriber->setup(_state);
            }
          }
        }

        const State * getState() const {
          return _state;
        }

        void publish(const State * state) {
          if (_setState(state)) {
            for (auto subscriber : _subscribers) {
              if (subscriber) {
                subscriber->onPublish(_state);
              }
            }
          }
        }

      private:

        const Subscribers _subscribers;
        const State * _state;
        unsigned long _uid;

        bool _setState(const State * state, bool force = false) {
          unsigned long uid = state->getUid();
          if (force || uid != _uid) {
            _uid = uid;
            _state = state;
            return true;
          }
          return false;
        }

    };

  }
}
