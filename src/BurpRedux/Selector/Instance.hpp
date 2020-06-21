#pragma once

#include <functional>
#include "../Publisher/Instance.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Selector {

    template <class Input, class Output, size_t size>
    class Instance : public Interface<Input, Output> {
      
      public:

        using f_select = std::function<const Output * (const Input * input)>;

        Instance(f_select select, const Input * input) :
          outputPublisher(select(input)),
          select(select)
        {}

        void onPublish(const Input * input) override {
          publish(select(input));
        }

        bool subscribe(Subscriber<Output> * subscriber) override {
          return outputPublisher.subscribe(subscriber);
        }

        const Output * getState() const override {
          return outputPublisher.getState();
        }
        
        size_t getSubscriberCount() const override {
          return outputPublisher.getSubscriberCount();
        }
        
      private:

        Publisher::Instance<Output, size> outputPublisher;
        f_select select;

        void publish(const Output * output) override {
          outputPublisher.publish(output);
        }

    };


  }
}