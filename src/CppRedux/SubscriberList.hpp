#pragma once

#include <array>
#include "Subscriber.hpp"

namespace CppRedux {

  template <size_t size>
  using Subscribers = std::array<Subscriber *, size>;

  template <size_t size>
  class SubscriberList : public Subscriber {

    public:

      SubscriberList(Subscribers<size> subscribers)
        : _subscribers(subscribers)
      {}

      void notify() override {
        for (auto subscriber : _subscribers) {
          subscriber->notify();
        }
      }

    private:

      const Subscribers<size>  _subscribers;

  };

}
