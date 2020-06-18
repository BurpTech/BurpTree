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
    using Reducer = Reducer::Reducer;
    using Subscriber = Subscriber::Subscriber;

    const Payload payload = {
      1234
    };
    const Action action(ActionType::ACTION, &payload);
    const Reducer reducer;
    Subscriber subscriber;
    CppRedux::Store<State, Action> store;

    Module tests("Store", [](Describe & describe) {

        describe.loop([]() {
            store.loop();
            subscriber.loop();
        });

        describe.before([]() {
            const State * state = new State(0, 0);
            store.setReducer(&reducer, state);
            store.setSubscriber(&subscriber);
        });

        describe.after([]() {
            delete store.getState();
        });

        describe.it("should initialise the state", []() {
            const State * state = store.getState();
            TEST_ASSERT_EQUAL(0, state->data1);
            TEST_ASSERT_EQUAL(0, state->data2);
        });

        describe.async("dispatch", [](Async & async, f_done & done) {
            store.dispatch(action);
            subscriber.callbackOnce([&]() {
                async.it("should update the state and notify", []() {
                    const State * state = store.getState();
                    TEST_ASSERT_EQUAL(1234, state->data1);
                    TEST_ASSERT_EQUAL(0, state->data2);
                });
                done();
            });
        });

    });

  }
}
