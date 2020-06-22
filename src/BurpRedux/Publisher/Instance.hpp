#pragma once

#include <array>
#include "Interface.hpp"

namespace BurpRedux {
  namespace Publisher {

    template <class State, size_t size>
    class Instance : Interface<State> {

      public:

        Instance() :
          _subscribers({}),
          _count(0),
          _overSubscribed(false),
          _state(nullptr)
        {}

        void setup(const State * state) {
          _state = state;
        }

        bool subscribe(Subscriber<State> * subscriber) override {
          if (_count < size) {
            _subscribers[_count] = subscriber;
            _count++;
            return true;
          }
          _overSubscribed = true;
          return false;
        }

        const State * getState() const override {
          return _state;
        }

        size_t getSubscriberCount() const override {
          return _count;
        }

        size_t getSubscriberMax() const override {
          return size;
        }

        bool isOverSubscribed() const override {
          return _overSubscribed;
        }

        void publish(const State * state) override {
          if (_state != state) {
            _state = state;
            for (auto subscriber : _subscribers) {
              if (subscriber) {
                subscriber->onPublish(_state);
              }
            }
          }
        }

      private:

        std::array<Subscriber<State> *, size> _subscribers;
        size_t _count;
        bool _overSubscribed;
        const State * _state;

    };

  }
}
