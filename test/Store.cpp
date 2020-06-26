#include "../src/BurpRedux.hpp"
#include <cstdio>
#include <unity.h>
#include "Action.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Store.hpp"

namespace BurpReduxTest {

  using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION>;
  using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION>;
  using Store = BurpRedux::Store::Instance<State, Params, 1>;

  Creator creator;
  Reducer reducer(creator, deserialize);
  Subscriber<State> subscriber;
  Store store(reducer, Store::Subscribers({
      &subscriber
  }));

  Module storeTests("Store", [](Describe & describe) {

      describe.setup([]() {
          StaticJsonDocument<256> doc;
          doc[data1Field] = 1;
          Params params = {0, 2};
          store.deserialize(doc.as<JsonObject>(), params);
      });

      describe.loop([]() {
          store.loop();
      });

      describe.it("should initialise the state", []() {
          const State * state = store.getState();
          TEST_ASSERT_EQUAL(1, state->data1);
          TEST_ASSERT_EQUAL(2, state->data2);
      });

      describe.describe("dispatch", [](Describe & describe) {
          describe.before([](f_done & done) {
              subscriber.callbackOnce(done);
              store.dispatch(Action({
                  3, 4
              }));
          });

          describe.it("should update the state and notify", []() {
              TEST_ASSERT_EQUAL(subscriber.state, store.getState());
              TEST_ASSERT_EQUAL(3, subscriber.state->data1);
              TEST_ASSERT_EQUAL(4, subscriber.state->data2);
          });

          describe.describe("then serialize", [](Describe & describe) {
              describe.it("should serialize the state", []() {
                  StaticJsonDocument<256> doc;
                  store.getState()->serialize(doc.to<JsonObject>());
                  TEST_ASSERT_EQUAL(3, doc[data1Field].as<int>());
              });
          });
      });

  });

}
