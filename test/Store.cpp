#include "../src/BurpRedux.hpp"
#include <unity.h>
#include "Action.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Store.hpp"

namespace BurpReduxTest {

  using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION>;
  using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION>;
  using Store = BurpRedux::Store::Instance<State, 1>;

  Reducer reducer(create<State, Params>);
  Subscriber<State> subscriber;
  Store store(reducer, Store::Subscribers({
      &subscriber
  }));

  const State * initialState = new State({
      0, 0
  });

  Module storeTests("Store", [](Describe & describe) {

      describe.setup([]() {
          store.setup(initialState);
      });

      describe.loop([]() {
          store.loop();
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
                  TEST_ASSERT_EQUAL(1, state->data1);
                  TEST_ASSERT_EQUAL(2, state->data2);
              });
              done();
          });
          store.dispatch(Action({
              1, 2
          }));
      });

  });

}
