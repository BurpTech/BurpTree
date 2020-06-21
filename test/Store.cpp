#include "../src/CppRedux.hpp"
#include <unity.h>
#include "Reducer.hpp"
#include "Subscriber.hpp"
#include "Store.hpp"

namespace CppReduxTest {
  namespace Store {

    using ActionType = Action::ActionType;
    using Payload = Action::Payload;
    using Action = Action::Action;
    using State = Reducer::State;
    using Subscriber = Subscriber::Subscriber<State>;

    const Payload payload = {
      1234
    };
    const Action action(ActionType::ACTION, &payload);
    Subscriber subscriber;
    const State * initialState = new State(0, 0);
    CppRedux::Store::Instance<State, Action, 1> store(Reducer::reducer, initialState);

    Module tests("Store", [](Describe & describe) {

        describe.loop([]() {
            store.loop();
        });

        describe.before([]() {
            store.subscribe(&subscriber);
        });

        describe.after([]() {
            delete store.getState();
        });

        describe.it("should initialise the state", []() {
            TEST_ASSERT_EQUAL(initialState, store.getState());
        });

        describe.async("dispatch", [](Async & async, f_done & done) {
            subscriber.callbackOnce([&](const State * state) {
                async.it("should update the state and notify", [&]() {
                    TEST_ASSERT_EQUAL(state, store.getState());
                    TEST_ASSERT_EQUAL(1234, state->data1);
                    TEST_ASSERT_EQUAL(0, state->data2);
                });
                done();
            });
            store.dispatch(action);
        });

    });

  }
}
