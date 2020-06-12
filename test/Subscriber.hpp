# pragma once

#include <functional>
#include "../src/CppRedux/Subscriber.hpp"

namespace CppReduxTest {
  namespace Subscriber {

    class Subscriber : public CppRedux::Subscriber {

      public:

        using f_cb = std::function<void()>;

        Subscriber() :
          _cb(nullptr),
          _callback(false)
        {}

        void notify() override {
          _callback = true;
        }

        void loop() {
          if (_callback) {
            _callback = false;
            if (_cb) {
              f_cb cb = _cb;
              _cb = nullptr;
              cb();
            }
          }
        }

        void callbackOnce(f_cb cb) {
          _cb = cb;
        }

      private:

        f_cb _cb;
        bool _callback;

    };

  }
}
