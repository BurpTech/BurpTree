#pragma once

#include <array>
#include "../State.hpp"
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
          // All states must implement the State interface
          // to enable us to compare states so that we
          // know when a state has changed. Simply checking pointers
          // is not good for this as we don't know that pointers
          // won't be reused (eg in a memory pool). Batching
          // actions mean that the State can change many times
          // between publishing, so any solution that rotates/reuses
          // identifying features like pointers will be problematic.
          // The State interface requires a comparison method
          // so an implementer can choose their own solution to this
          // problem
          const BurpRedux::State<State> * next = state;
          if (next->isNew(_state)) {
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
