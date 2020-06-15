#include <unity.h>
#include "State.hpp"

namespace CppReduxTest {
  namespace State {

    Module tests("State", [](Describe & describe) {

        describe.describe("when constructed with a nullptr", [](Describe & describe) {
            describe.it("should have sequence 0", []() {
                CppRedux::State state(nullptr);
                TEST_ASSERT_EQUAL(0, state.cppReduxSequence);
            });
        });

        describe.describe("when constructed with an existing state", [](Describe & describe) {
            describe.it("should increment the sequence", []() {
                CppRedux::State state1(nullptr);
                CppRedux::State state2(&state1);
                CppRedux::State state3(&state2);
                CppRedux::State state4(&state1);
                TEST_ASSERT_EQUAL(0, state1.cppReduxSequence);
                TEST_ASSERT_EQUAL(1, state2.cppReduxSequence);
                TEST_ASSERT_EQUAL(2, state3.cppReduxSequence);
                TEST_ASSERT_EQUAL(1, state4.cppReduxSequence);
            });
        });

    });

  }
}
