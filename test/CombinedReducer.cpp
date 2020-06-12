#include <unity.h>
#include "CombinedReducer.hpp"

namespace CppReduxTest {
  namespace CombinedReducer {

    const Payload payload = {
      "hello"
    };
    const Action action(ActionType::ACTION, &payload);
    const Reducer firstReducer;
    const Reducer secondReducer;
    const ReducerMap map(firstReducer, secondReducer);
    const CppRedux::CombinedReducer<CombinedState, Action> combinedReducer(map);

    const CombinedState * state = nullptr;

    Module tests("CombinedReducer", [](Describe & describe) {

        describe.describe("init", [](Describe & describe) {
            describe.before([]() {
                const State initialFirst("first");
                const State initialSecond("second");
                const CombinedState initialState(&initialFirst, &initialSecond);
                state = combinedReducer.init(nullptr, initialState);
            });

            describe.it("should initialise a combined state", []() {
                TEST_ASSERT_EQUAL(0, state->reduxSequence);
                TEST_ASSERT_EQUAL(0, state->first->reduxSequence);
                TEST_ASSERT_EQUAL(0, state->second->reduxSequence);
                TEST_ASSERT_EQUAL_STRING("first", state->first->data);
                TEST_ASSERT_EQUAL_STRING("second", state->second->data);
            });

            describe.describe("and init again", [](Describe & describe) {
                describe.before([]() {
                    const State initialFirst("first");
                    const State initialSecond("second");
                    const CombinedState initialState(&initialFirst, &initialSecond);
                    state = combinedReducer.init(state, initialState);
                });

                describe.it("should initialise a new combined state", []() {
                    TEST_ASSERT_EQUAL(1, state->reduxSequence);
                    TEST_ASSERT_EQUAL(1, state->first->reduxSequence);
                    TEST_ASSERT_EQUAL(1, state->second->reduxSequence);
                    TEST_ASSERT_EQUAL_STRING("first", state->first->data);
                    TEST_ASSERT_EQUAL_STRING("second", state->second->data);
                });

                describe.describe("then reduce", [](Describe & describe) {
                    describe.before([]() {
                        state = combinedReducer.reduce(state, action);
                    });

                    describe.it("should create a new combined state", []() {
                        TEST_ASSERT_EQUAL(2, state->reduxSequence);
                        TEST_ASSERT_EQUAL(2, state->first->reduxSequence);
                        TEST_ASSERT_EQUAL(2, state->second->reduxSequence);
                        TEST_ASSERT_EQUAL_STRING("hello", state->first->data);
                        TEST_ASSERT_EQUAL_STRING("hello", state->second->data);
                    });
                });
            });
        });

    });

  }

}
