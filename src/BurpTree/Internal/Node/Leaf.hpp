#pragma once

#include "../Publisher.hpp"
#include "Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Node {

      template <class _Factory, size_t subscriberCount>
      class Leaf : public Base<_Factory, subscriberCount> {

        public:

          using StateInterface = State::Interface;
          using Base = Node::Base<_Factory, subscriberCount>;
          using Factory = typename Base::Factory;
          using Subscribers = typename Base::Subscribers;
          using Id = Node::Id;

          Leaf(const Id id, Factory & factory, const Subscribers & subscribers) :
            Base(factory, subscribers),
            _id(id),
            _notify(false)
          {}

          const Id getId() const {
            return _id;
          }

          const StateInterface * update(const Id changed) override {
            if (changed == _id) {
              _notify = true;
              return this->_factory.getState();
            }
            return nullptr;
          }

          void notify() override {
            if (_notify) {
              _notify = false;
              this->_publisher.notify(this->_factory.getState());
            }
          }

        private:

          const Id _id;
          bool _notify;

      };

    }
  }
}
