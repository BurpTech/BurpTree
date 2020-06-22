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
          _state(nullptr),
          _uid(0)
        {}

        void setup(const State * state) {
          _setState(state, true);
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
          if (_setState(state)) {
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
        unsigned long _uid;

        bool _setState(const State * state, bool force = false) {
          // All states must implement the State interface
          // to expose a unique id. We will use this to
          // check if a state has changed. Simply checking pointers
          // is not good for this as we don't know that pointers
          // won't be reused (eg in a memory pool). Batching
          // actions mean that the State can change many times
          // between publishing, so any solution that rotates/reuses
          // identifying features like pointers will be problematic.
          // We have to get and store the unique id alongside the current
          // state as the current state may well have been cleaned
          // up by the time we need to check it.
          const BurpRedux::State * next = state;
          unsigned long uid = next->getUid();
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
