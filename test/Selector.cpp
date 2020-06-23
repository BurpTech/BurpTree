#include <unity.h>
#include <functional>
#include "../src/BurpRedux.hpp"
#include "Subscriber.hpp"
#include "Selector.hpp"

namespace BurpReduxTest {

  using namespace BurpRedux;

  class Child : public BurpRedux::State::Interface {
    public:
      unsigned long getUid() const override {
        return (unsigned long)this;
      }
  };

  class Parent : public BurpRedux::State::Interface {
    public:
      const Child * child;
      Parent(const Child * child) :
        child(child)
      {}
      unsigned long getUid() const override {
        return (unsigned long)this;
      }
  };

  using Publisher = BurpRedux::Publisher::Instance<Parent, 1>;
  using Selector = BurpRedux::Selector::Instance<Parent, Child, 1>;

  Child c1;
  Child c2;
  Parent p1(&c1);
  Parent p2(&c1);
  Parent p3(&c2);

  Subscriber<Child> childSubscriber;
  Selector selector([](const Parent * parent) {
      return parent->child;
  }, Selector::Subscribers({
      &childSubscriber
  }));
  Publisher publisher({
      &selector
  });

  const Child * callbackChild;

  Module selectorTests("Selector", [](Describe & describe) {
      describe.setup([]() {
          publisher.setup(&p1);
          selector.setup(publisher.getState());
      });

      describe.describe("with the initial state", [](Describe & describe) {
          describe.it("should have the correct state", []() {
              TEST_ASSERT_EQUAL(&c1, selector.getState());
          });

          describe.describe("then with a new parent but the same child", [](Describe & describe) {
              describe.before([]() {
                  callbackChild = nullptr;
                  childSubscriber.callbackOnce([&](const Child * child) {
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
                      childSubscriber.callbackOnce([&](const Child * child) {
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
