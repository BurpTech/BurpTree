#include <unity.h>
#include "../src/BurpRedux/Sequence/Instance.hpp"
#include "State.hpp"
#include "Sequence.hpp"

namespace BurpReduxTest {

  BurpRedux::Sequence::Instance<State, Params> sequence;

  Module sequenceTests("Sequence", [](Describe & describe) {

      describe.it("should start with an id of zero", []() {
          TEST_ASSERT_EQUAL(0, sequence.getId());
      });

      describe.describe("newState", [](Describe & describe) {

          describe.before([]() {
              const Params params = {
                0,
                1
              };
              sequence.newState(&params);
          });

          describe.it("should increment the id", []() {
              TEST_ASSERT_EQUAL(1, sequence.getId());
          });

          describe.it("should construct the state", []() {
              const State * state = sequence.getState();
              TEST_ASSERT_EQUAL(0, state->data1);
              TEST_ASSERT_EQUAL(1, state->data2);
          });

          describe.describe("then newState again", [](Describe & describe) {

              describe.before([]() {
                  const Params params = {
                    2,
                    3
                  };
                  sequence.newState(&params);
              });

              describe.it("should increment the id", []() {
                  TEST_ASSERT_EQUAL(2, sequence.getId());
              });

              describe.it("should construct the state", []() {
                  const State * state = sequence.getState();
                  TEST_ASSERT_EQUAL(2, state->data1);
                  TEST_ASSERT_EQUAL(3, state->data2);
              });

          });

      });

  });

}
