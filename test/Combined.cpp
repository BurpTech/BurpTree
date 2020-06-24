#include <unity.h>
#include "../src/BurpRedux.hpp"
#include "Action.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Combined.hpp"

namespace BurpReduxTest {

  namespace One {
    Subscriber<State> subscriber;
    using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION_1>;
    using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION_1>;
    Reducer reducer(create<State, Params>);
    BURP_REDUX_SUB_STATE(
        one,
        CombinedState,
        CombinedParams,
        State,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  namespace Two {
    Subscriber<State> subscriber;
    using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION_2>;
    using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION_2>;
    Reducer reducer(create<State, Params>);
    BURP_REDUX_SUB_STATE(
        two,
        CombinedState,
        CombinedParams,
        State,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  namespace Three {
    Subscriber<State> subscriber;
    using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION_3>;
    using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION_3>;
    Reducer reducer(create<State, Params>);
    BURP_REDUX_SUB_STATE(
        three,
        CombinedState,
        CombinedParams,
        State,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  using CombinedReducer = BurpRedux::CombinedReducer<CombinedState, CombinedParams, 3>;
  CombinedReducer combinedReducer(create<CombinedState, CombinedParams>, CombinedReducer::Map({
      &One::reducerMapping,
      &Two::reducerMapping,
      &Three::reducerMapping
  }));

  using Store = BurpRedux::Store::Instance<CombinedState, 3>;
  Store combinedStore(combinedReducer, Store::Subscribers({
      &One::selector,
      &Two::selector,
      &Three::selector
  }));


  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {
          const CombinedState * initial = new CombinedState({
              new State({1, 2}),
              new State({3, 4}),
              new State({5, 6})
          });
          combinedStore.setup(initial);
      });

      describe.loop([]() {
          combinedStore.loop();
      });

      describe.after([]() {
          const CombinedState * state = combinedStore.getState();
          delete state->three;
          delete state->two;
          delete state->one;
          delete state;
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(1, One::selector.getState()->data1);
          TEST_ASSERT_EQUAL(2, One::selector.getState()->data2);
          TEST_ASSERT_EQUAL(3, Two::selector.getState()->data1);
          TEST_ASSERT_EQUAL(4, Two::selector.getState()->data2);
          TEST_ASSERT_EQUAL(5, Three::selector.getState()->data1);
          TEST_ASSERT_EQUAL(6, Three::selector.getState()->data2);
      });

      describe.describe("on action", [](Describe & describe) {
          describe.before([](f_done & done) {
              combinedStore.dispatch(One::Action({7, 8}));
              One::subscriber.callbackOnce([&](const State * _) {
                  done();
              });
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, One::subscriber.count);
              TEST_ASSERT_EQUAL(7, One::subscriber.state->data1);
              TEST_ASSERT_EQUAL(8, One::subscriber.state->data2);

              TEST_ASSERT_EQUAL(0, Two::subscriber.count);
              TEST_ASSERT_EQUAL(3, Two::subscriber.state->data1);
              TEST_ASSERT_EQUAL(4, Two::subscriber.state->data2);

              TEST_ASSERT_EQUAL(0, Three::subscriber.count);
              TEST_ASSERT_EQUAL(5, Three::subscriber.state->data1);
              TEST_ASSERT_EQUAL(6, Three::subscriber.state->data2);
          });
      });

  });

}
