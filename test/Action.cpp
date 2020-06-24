#include <unity.h>
#include "../src/BurpRedux/Action/Instance.hpp"
#include "State.hpp"
#include "Action.hpp"

namespace BurpReduxTest {

  const Params params =  {1, 2};
  const BurpRedux::Action::Instance<Params, ActionType::ACTION>  action(params);

  Module actionTests("Action", [](Describe & describe) {

      describe.describe("getType", [](Describe & describe) {
          describe.it("should return the type", []() {
              TEST_ASSERT_EQUAL(ActionType::ACTION, action.getType());
          });
      });

      describe.describe("getParams", [](Describe & describe) {
          describe.it("should return the params", []() {
              TEST_ASSERT_EQUAL(&params, &(action.getParams()));
          });
      });

  });

}
