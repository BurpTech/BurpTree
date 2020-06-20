#include <unity.h>
#include <functional>
#include "../src/CppRedux/Selector.hpp"
#include "../src/CppRedux/Subscriber.hpp"
#include "Selector.hpp"

namespace CppReduxTest {
  namespace Selector {

    using namespace CppRedux;

    class Child {};

    class Parent {
      public:
        const Child * child;
        Parent(const Child * child) :
          child(child)
        {}
    };

    class Store {
      public:
        Store() :
          _parent(nullptr),
          _subscriber(nullptr)
        {}

        const Parent * getState() const {
          return _parent;
        }

        void setState(const Parent * parent) {
          _parent = parent;
          if (_subscriber) {
            _subscriber->notify();
          }
        }

        void setSubscriber(CppRedux::Subscriber * subscriber) {
          _subscriber = subscriber;
        }

      private:
        const Parent * _parent;
        CppRedux::Subscriber * _subscriber;

    };

    using Selector = CppRedux::Selector<Store, Parent, Child>;

    class Subscriber : public CppRedux::Subscriber {
      public:
        using f_cb = std::function<void()>;

        Subscriber() :
          _cb(nullptr)
        {}

        void notify() override {
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

    Store store;
    Selector selector(store, [](const Parent * parent) {
        return parent->child;
    });
    Subscriber subscriber;

    Child c1;
    Child c2;
    Parent p1(&c1);
    Parent p2(&c1);
    Parent p3(&c2);

    bool calledBack;

    Module tests("Selector", [](Describe & describe) {
        describe.setup([]() {
            store.setSubscriber(&selector);
            selector.setSubscriber(&subscriber);
        });

        describe.describe("with a new state", [](Describe & describe) {
            describe.before([]() {
                calledBack = false;
                subscriber.callbackOnce([&]() {
                    calledBack = true;
                });
                store.setState(&p1);
            });

            describe.it("should notify and have the correct state", []() {
                TEST_ASSERT_TRUE(calledBack);
                TEST_ASSERT_EQUAL(&c1, selector.getState());
            });

            describe.describe("then with a new parent but the same child", [](Describe & describe) {
                describe.before([]() {
                    calledBack = false;
                    subscriber.callbackOnce([&]() {
                        calledBack = true;
                    });
                    store.setState(&p2);
                });

                describe.it("should not notify or change state", []() {
                    TEST_ASSERT_FALSE(calledBack);
                    TEST_ASSERT_EQUAL(&c1, selector.getState());
                });

                describe.describe("then with a new parent and a new child", [](Describe & describe) {
                    describe.before([]() {
                        calledBack = false;
                        subscriber.callbackOnce([&]() {
                            calledBack = true;
                        });
                        store.setState(&p3);
                    });

                    describe.it("should notify and change state", []() {
                        TEST_ASSERT_TRUE(calledBack);
                        TEST_ASSERT_EQUAL(&c2, selector.getState());
                    });
                });
            });
        });
    });

  }
}
