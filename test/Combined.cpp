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
    Creator creator;
    Reducer reducer(creator);
    BURP_REDUX_SUB_STATE(
        one,
        CombinedState,
        CombinedParams,
        State
    );
    ReducerMapping reducerMapping(reducer);
    Selector<1> selector({
        &subscriber
    });
  }

  namespace Two {
    Subscriber<State> subscriber;
    using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION_2>;
    using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION_2>;
    Creator creator;
    Reducer reducer(creator);
    BURP_REDUX_SUB_STATE(
        two,
        CombinedState,
        CombinedParams,
        State
    );
    ReducerMapping reducerMapping(reducer);
    Selector<1> selector({
        &subscriber
    });
  }

  namespace Three {
    Subscriber<State> subscriber;
    using Action = BurpRedux::Action::Instance<Params, ActionType::ACTION_3>;
    using Reducer = BurpRedux::Reducer::Instance<State, Params, ActionType::ACTION_3>;
    Creator creator;
    Reducer reducer(creator);
    BURP_REDUX_SUB_STATE(
        three,
        CombinedState,
        CombinedParams,
        State
    );
    ReducerMapping reducerMapping(reducer);
    Selector<1> selector({
        &subscriber
    });
  }

  template <size_t mappingCount>
  using CombinedReducer = BurpRedux::CombinedReducer<CombinedState, CombinedParams, mappingCount>;
  CombinedCreator combinedCreator;
  CombinedReducer<3> combinedReducer(combinedCreator, CombinedReducer<3>::Map({
      &One::reducerMapping,
      &Two::reducerMapping,
      &Three::reducerMapping
  }));

  template <size_t subscriberCount>
  using Store = BurpRedux::Store::Instance<CombinedState, subscriberCount>;
  Store<3> combinedStore(combinedReducer, Store<3>::Subscribers({
      &One::selector,
      &Two::selector,
      &Three::selector
  }));


  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {
          const CombinedState * initial = combinedCreator.init({
              One::creator.init({1, 2}),
              Two::creator.init({3, 4}),
              Three::creator.init({5, 6})
          });
          combinedStore.setup(initial);
      });

      describe.loop([]() {
          combinedStore.loop();
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(1, One::selector.getState()->data1);
          TEST_ASSERT_EQUAL(2, One::selector.getState()->data2);
          TEST_ASSERT_EQUAL(3, Two::selector.getState()->data1);
          TEST_ASSERT_EQUAL(4, Two::selector.getState()->data2);
          TEST_ASSERT_EQUAL(5, Three::selector.getState()->data1);
          TEST_ASSERT_EQUAL(6, Three::selector.getState()->data2);
      });

      describe.describe("then dispatch action one", [](Describe & describe) {
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

          describe.describe("then dispatch action two", [](Describe & describe) {
              describe.before([](f_done & done) {
                  combinedStore.dispatch(Two::Action({9, 10}));
                  Two::subscriber.callbackOnce([&](const State * _) {
                      done();
                  });
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(1, One::subscriber.count);
                  TEST_ASSERT_EQUAL(7, One::subscriber.state->data1);
                  TEST_ASSERT_EQUAL(8, One::subscriber.state->data2);

                  TEST_ASSERT_EQUAL(1, Two::subscriber.count);
                  TEST_ASSERT_EQUAL(9, Two::subscriber.state->data1);
                  TEST_ASSERT_EQUAL(10, Two::subscriber.state->data2);

                  TEST_ASSERT_EQUAL(0, Three::subscriber.count);
                  TEST_ASSERT_EQUAL(5, Three::subscriber.state->data1);
                  TEST_ASSERT_EQUAL(6, Three::subscriber.state->data2);
              });

              describe.describe("then dispatch action three", [](Describe & describe) {
                  describe.before([](f_done & done) {
                      combinedStore.dispatch(Three::Action({11, 12}));
                      Three::subscriber.callbackOnce([&](const State * _) {
                          done();
                      });
                  });

                  describe.it("should notify the correct subscribers with the correct state", []() {
                      TEST_ASSERT_EQUAL(1, One::subscriber.count);
                      TEST_ASSERT_EQUAL(7, One::subscriber.state->data1);
                      TEST_ASSERT_EQUAL(8, One::subscriber.state->data2);

                      TEST_ASSERT_EQUAL(1, Two::subscriber.count);
                      TEST_ASSERT_EQUAL(9, Two::subscriber.state->data1);
                      TEST_ASSERT_EQUAL(10, Two::subscriber.state->data2);

                      TEST_ASSERT_EQUAL(1, Three::subscriber.count);
                      TEST_ASSERT_EQUAL(11, Three::subscriber.state->data1);
                      TEST_ASSERT_EQUAL(12, Three::subscriber.state->data2);
                  });
              });
          });
      });

  });

}
