#include <unity.h>
#include "Action.hpp"

namespace BurpReduxTest {

  const Payload payload = {
    1234
  };
  const Action action(ActionType::ACTION, &payload);

  Module actionTests("Action", [](Describe & describe) {

      describe.describe("type", [](Describe & describe) {
          describe.it("should return the type", []() {
              TEST_ASSERT_EQUAL(ActionType::ACTION, action.type);
          });
      });

      describe.describe("payload()", [](Describe & describe) {
          describe.it("should return the payload", []() {
              TEST_ASSERT_EQUAL(1234, action.payload<Payload>()->data);
          });
      });

  });

}
