#include <unity.h>
#include "Action.hpp"

namespace CppReduxTest {
  namespace Action {

    const Payload payload = {
      "hello"
    };
    const Action action(ActionType::ACTION, &payload);

    Module tests("Action", [](Describe & describe) {

        describe.describe("type", [](Describe & describe) {
            describe.it("should return the type", []() {
                TEST_ASSERT_EQUAL(ActionType::ACTION, action.type);
            });
        });

        describe.describe("payload()", [](Describe & describe) {
            describe.it("should return the payload", []() {
                TEST_ASSERT_EQUAL_STRING("hello", action.payload<Payload>()->data);
            });
        });

    });

  }
}
