#pragma once

#include "../State/Interface.hpp"
#include "../CombinedState/Interface.hpp"
#include "../Publisher/Instance.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    template <size_t size>
    class Instance : public Interface {
      
      public:

        using State = BurpRedux::State::Interface;
        using CombinedState = BurpRedux::CombinedState::Interface;
        using Subscriber = Subscriber::Interface;
        using Subscribers = std::array<Subscriber *, size>;

        Instance(Subscribers subscribers) :
          outputPublisher(subscribers)
        {}

        void setup(const State * input) override {
          outputPublisher.setup(select(input));
        }

        void onPublish(const Input * input) override {
          publish(select(input));
        }

        const Output * getState() const override {
          return outputPublisher.getState();
        }
        
      private:

        Publisher::Instance<Output, size> outputPublisher;

        void publish(const Output * output) override {
          outputPublisher.publish(output);
        }

    };


  }
}
