#pragma once

#include "../Publisher/Instance.hpp"
#include "Subscriber.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input, class Output, size_t subscriberCount>
    class Instance : public Subscriber<Input> {
      
      public:

        using States = State::List::Get;
        using Index = States::Index;
        using Publisher = Publisher::Instance<Output, subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;

        Instance(Subscribers subscribers) :
          publisher(subscribers)
        {}

        void init(const size_t index) override {
          _index = index;
        }

        void setup(const Input * input) override {
          publisher.setup(_select(input));
        }

        void onPublish(const Input * input) override {
          publish(_select(input));
        }

        const Output * getState() const {
          return publisher.getState();
        }
        
      private:

        Index _index;
        Publisher publisher;

        void publish(const Output * output) {
          publisher.publish(output);
        }

        const Output * _select(const Input * input) {
          const States * states = input;
          return states->get(_index)->template as<Output>();
        }

    };


  }
}
