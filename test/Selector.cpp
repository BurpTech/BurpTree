#include <unity.h>
#include <functional>
#include "../src/BurpRedux.hpp"
#include "Subscriber.hpp"
#include "Selector.hpp"

namespace BurpReduxTest {
  namespace Selector {

    using namespace BurpRedux;

    class Child {};

    using Subscriber = Subscriber::Subscriber<Child>;

    class Parent {
      public:
        const Child * child;
        Parent(const Child * child) :
          child(child)
        {}
    };

    using Publisher = BurpRedux::Publisher::Instance<Parent, 1>;
    using Selector = BurpRedux::Selector::Instance<Parent, Child, 1>;

    Child c1;
    Child c2;
    Parent p1(&c1);
    Parent p2(&c1);
    Parent p3(&c2);

    Publisher publisher(&p1);
    Selector selector([](const Parent * parent) {
        return parent->child;
    }, publisher.getState());
    Subscriber subscriber;

    const Child * callbackChild;

    Module tests("Selector", [](Describe & describe) {
        describe.setup([]() {
            publisher.subscribe(&selector);
            selector.subscribe(&subscriber);
        });

        describe.describe("with the initial state", [](Describe & describe) {
            describe.it("should have the correct state", []() {
                TEST_ASSERT_EQUAL(&c1, selector.getState());
            });

            describe.describe("then with a new parent but the same child", [](Describe & describe) {
                describe.before([]() {
                    callbackChild = nullptr;
                    subscriber.callbackOnce([&](const Child * child) {
                        callbackChild = child;
                    });
                    publisher.publish(&p2);
                });

                describe.it("should not notify or change state", []() {
                    TEST_ASSERT_NULL(callbackChild);
                    TEST_ASSERT_EQUAL(&c1, selector.getState());
                });

                describe.describe("then with a new parent and a new child", [](Describe & describe) {
                    describe.before([]() {
                        callbackChild = nullptr;
                        subscriber.callbackOnce([&](const Child * child) {
                            callbackChild = child;
                        });
                        publisher.publish(&p3);
                    });

                    describe.it("should notify and change state", []() {
                        TEST_ASSERT_EQUAL(&c2, callbackChild);
                        TEST_ASSERT_EQUAL(&c2, selector.getState());
                    });
                });
            });
        });
    });

  }
}
