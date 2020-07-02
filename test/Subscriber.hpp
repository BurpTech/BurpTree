# pragma once

#include <functional>
#include "../src/BurpTree.hpp"

namespace BurpTreeTest {

  class Subscriber : public BurpTree::Subscriber {

    public:

      using f_cb = std::function<void()>;

      const BurpTree::State * state;
      unsigned int count;

      Subscriber() :
        state(nullptr),
        count(0),
        _cb(nullptr)
      {}

      void setup(const BurpTree::State * initial) override {
        state = initial;
      }

      void onPublish(const BurpTree::State * next) override {
        count++;
        state = next;
        if (_cb) {
          f_cb cb = _cb;
          _cb = nullptr;
          cb();
        }
      }

      void callbackOnce(f_cb cb) {
        _cb = cb;
      }

    private:

      f_cb _cb;

  };

}
