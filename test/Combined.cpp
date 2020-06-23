#include "../src/BurpRedux.hpp"
#include "Action.hpp"
#include "Subscriber.hpp"
#include "Reducer.hpp"
#include "Combined.hpp"

namespace BurpReduxTest {

  struct CombinedParams {
    const State * one;
    const State * two;
    const State * three;
  };

  class CombinedState : public BurpRedux::State::Interface {

    public:

      const BurpReduxTest::State * one;
      const BurpReduxTest::State * two;
      const BurpReduxTest::State * three;

      CombinedState(const CombinedParams & params) :
          one(params.one),
          two(params.two),
          three(params.three)
      {}

      unsigned long getUid() const override {
        return (unsigned long)this;
      }

  };

  namespace One {
    Subscriber<State> subscriber;
    Reducer<ActionType::ACTION_1> reducer;
    BURP_REDUX_SUB_STATE(
        one,
        CombinedState,
        CombinedParams,
        State,
        Action,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  namespace Two {
    Subscriber<State> subscriber;
    Reducer<ActionType::ACTION_2> reducer;
    BURP_REDUX_SUB_STATE(
        two,
        CombinedState,
        CombinedParams,
        State,
        Action,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  namespace Three {
    Subscriber<State> subscriber;
    Reducer<ActionType::ACTION_3> reducer;
    BURP_REDUX_SUB_STATE(
        three,
        CombinedState,
        CombinedParams,
        State,
        Action,
        reducer,
        1, {
          &subscriber
        }
    );
  }

  const CombinedState * create(const CombinedState * previous, const CombinedParams & params) {
    delete previous;
    return new CombinedState(params);
  }

  using CombinedReducer = BurpRedux::CombinedReducer<CombinedState, CombinedParams, Action, 3>;
  CombinedReducer combinedReducer(create, CombinedReducer::Map({
      &One::reducerMapping,
      &Two::reducerMapping,
      &Three::reducerMapping
  }));

  using Store = BurpRedux::Store::Instance<CombinedState, Action, 3>;
  Store combinedStore(combinedReducer, Store::Subscribers({
      &One::selector,
      &Two::selector,
      &Three::selector
  }));


  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {
          combinedStore.setup(new CombinedState({
              new State(1, 1),
              new State(2, 2),
              new State(3, 3)
          }));
          One::selector.setup(combinedStore.getState());
          Two::selector.setup(combinedStore.getState());
          Three::selector.setup(combinedStore.getState());
      });

      describe.after([]() {
          delete Three::selector.getState();
          delete Two::selector.getState();
          delete One::selector.getState();
          delete combinedStore.getState();
      });

  });

}
