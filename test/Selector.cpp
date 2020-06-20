#include <unity.h>
#include "Selector.hpp"

namespace CppReduxTest {
  namespace Selector {

    class Child {
      public:
        const int data;
        Child(const int data) :
          data(data)
        {}
    };

    class Parent {
      public:
        const Child * child;
        Parent(const Child * child) :
          child(child)
        {}
    };

    CppRedux::Selector<Parent, Child> selector([](const Parent * parent) {
        return parent->child;
    });

    Module tests("Selector", [](Describe & describe) {
        describe.describe("check, get & reset", [](Describe & describe) {
            describe.it("check should only callback if the selected field has a new address", []() {
                int callbackCount = 0;
                const Child c1(5);
                const Parent p1(&c1);
                selector.check(&p1, [&](const Child * child) {
                    TEST_ASSERT_EQUAL(&c1, child);
                    callbackCount++;
                });
                TEST_ASSERT_EQUAL(1, callbackCount);
                TEST_ASSERT_EQUAL(&c1, selector.get());
                selector.check(&p1, [&](const Child * child) {
                    callbackCount++;
                });
                // callback should not have been called
                TEST_ASSERT_EQUAL(1, callbackCount);
                TEST_ASSERT_EQUAL(&c1, selector.get());
                const Parent p2(&c1);
                selector.check(&p2, [&](const Child * child) {
                    callbackCount++;
                });
                // callback should not have been called
                TEST_ASSERT_EQUAL(&c1, selector.get());
                TEST_ASSERT_EQUAL(1, callbackCount);
                const Child c2(10);
                const Parent p3(&c2);
                selector.check(&p3, [&](const Child * child) {
                    TEST_ASSERT_EQUAL(&c2, child);
                    callbackCount++;
                });
                TEST_ASSERT_EQUAL(2, callbackCount);
                TEST_ASSERT_EQUAL(&c2, selector.get());
                selector.reset();
                TEST_ASSERT_EQUAL(nullptr, selector.get());
            });
        });
    });

  }
}
