#include <unity.h>
#include "State.hpp"
#include "Reducer.hpp"
#include "Subscriber.hpp"
#include "Store.hpp"

namespace CppReduxTest {
  namespace Store {

    using ActionType = Action::ActionType;
    using Payload = Action::Payload;
    using Action = Action::Action;
    using State = State::State;
    using Reducer = Reducer::Reducer;
    using Subscriber = Subscriber::Subscriber;

    const Payload payload = {
      "hello"
    };
    const Action action(ActionType::ACTION, &payload);
    const Reducer reducer;
    Subscriber subscriber;
    CppRedux::Store<State, Action> store(reducer);

    Module tests("Store", [](Describe & describe) {

        describe.setup([]() {
            store.setSubscriber(&subscriber);
        });

        describe.loop([]() {
            store.loop();
            subscriber.loop();
        });

        describe.describe("init", [](Describe & describe) {
            describe.before([]() {
                const State state("initial");
                store.init(state);
            });

            describe.it("should initialise the state", []() {
                const State * state = store.getState();
                TEST_ASSERT_EQUAL(0, state->reduxSequence);
                TEST_ASSERT_EQUAL_STRING("initial", state->data);
            });

            describe.async("then dispatch an action", [](Async & async, f_done & done) {
                store.dispatch(action);
                subscriber.callbackOnce([&]() {
                    async.it("should update the state and notify", []() {
                        const State * state = store.getState();
                        TEST_ASSERT_EQUAL(1, state->reduxSequence);
                        TEST_ASSERT_EQUAL_STRING("hello", store.getState()->data);
                    });
                    done();
                });
            });
        });

    });

  }
}
