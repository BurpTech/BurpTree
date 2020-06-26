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
    Reducer reducer(creator, deserialize);
    BURP_REDUX_SUB_STATE(
        one,
        oneField,
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
    Reducer reducer(creator, deserialize);
    BURP_REDUX_SUB_STATE(
        two,
        twoField,
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
    Reducer reducer(creator, deserialize);
    BURP_REDUX_SUB_STATE(
        three,
        threeField,
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
          StaticJsonDocument<512> doc;
          doc[oneField][data1Field] = 1;
          doc[oneField][data2Field] = 2;
          doc[twoField][data1Field] = 3;
          doc[twoField][data2Field] = 4;
          doc[threeField][data1Field] = 5;
          doc[threeField][data2Field] = 6;
          combinedStore.deserialize(doc.as<JsonObject>());
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
              One::subscriber.callbackOnce(done);
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
                  Two::subscriber.callbackOnce(done);
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
                      Three::subscriber.callbackOnce(done);
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

                  describe.describe("then serialize", [](Describe & describe) {
                      describe.it("should serialize the state", []() {
                          StaticJsonDocument<512> doc;
                          combinedStore.getState()->serialize(doc.to<JsonObject>());
                          TEST_ASSERT_EQUAL(7, doc[oneField][data1Field].as<int>());
                          TEST_ASSERT_EQUAL(8, doc[oneField][data2Field].as<int>());
                          TEST_ASSERT_EQUAL(9, doc[twoField][data1Field].as<int>());
                          TEST_ASSERT_EQUAL(10, doc[twoField][data2Field].as<int>());
                          TEST_ASSERT_EQUAL(11, doc[threeField][data1Field].as<int>());
                          TEST_ASSERT_EQUAL(12, doc[threeField][data2Field].as<int>());
                      });
                  });
              });
          });
      });

  });

}
