#pragma once

#include "../Publisher/Instance.hpp"
#include "Subscriber.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input, class Output, size_t subscriberCount>
    class Instance : public Subscriber<Input> {
      
      public:

        using Index = Interface::Index;
        using StateList = State::List::Interface;
        using Publisher = Publisher::Instance<Output, subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;

        Instance(Subscribers subscribers) :
          publisher(subscribers)
        {}

        void init(const Index index) override {
          _index = index;
        }

        void setup(Input * input) override {
          publisher.setup(_select(input));
        }

        void onPublish(Input * input) override {
          publish(_select(input));
        }

        const Output * getState() const {
          return publisher.getState();
        }
        
      private:

        Index _index;
        Publisher publisher;

        void publish(Output * output) {
          publisher.publish(output);
        }

        Output * _select(Input * input) {
          StateList * list = input;
          return list->get(_index)->template as<Output>();
        }

    };


  }
}
