#include <unity.h>
#include "../src/BurpRedux.hpp"
#include "Constants.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Combined.hpp"

using Reducer = BurpRedux::Reducer::Instance;

namespace BurpReduxTest {

  using MapEntry = BurpRedux::Reducer::Combined::Map::Entry;
  using CombinedReducer = BurpRedux::Reducer::Combined::Instance<ReducerIndex::count>;
  using StateList = BurpRedux::State::List::Instance<ReducerId::count>;
  using Store = BurpRedux::Store::Instance<CombinedReducer::State, 2>;

  namespace A {

    using CombinedReducer = BurpRedux::Reducer::Combined::Instance<ReducerIndex::count>;
    using CombinedSelector = BurpRedux::Selector::Instance<BurpReduxTest::CombinedReducer::State, CombinedReducer::State, 3>;
    using Selector = BurpRedux::Selector::Instance<CombinedReducer::State, State, 1>;

    namespace A {
      Creator creator;
      Reducer reducer(ReducerId::aa);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    namespace B {
      Creator creator;
      Reducer reducer(ReducerId::ab);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    namespace C {
      Creator creator;
      Reducer reducer(ReducerId::ac);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    CombinedReducer reducer({
      A::entry,
      B::entry,
      C::entry
    });

    CombinedSelector selector({
      &A::selector,
      &B::selector,
      &C::selector
    });

    BurpReduxTest::CombinedReducer::Entry entry = {
      field,
      &reducer,
      &selector
    };

  }

  namespace B {

    using CombinedReducer = BurpRedux::Reducer::Combined::Instance<ReducerIndex::count>;
    using CombinedSelector = BurpRedux::Selector::Instance<BurpReduxTest::CombinedReducer::State, CombinedReducer::State, 3>;
    using Selector = BurpRedux::Selector::Instance<CombinedReducer::State, State, 1>;

    namespace A {
      Creator creator;
      Reducer reducer(ReducerId::ba);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    namespace B {
      Creator creator;
      Reducer reducer(ReducerId::bb);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    namespace C {
      Creator creator;
      Reducer reducer(ReducerId::bc);
      Subscriber subscriber;
      Selector selector({
          &subscriber
      });
      CombinedReducer::Entry entry = {
        field,
        &reducer,
        &selector
      };
    }

    CombinedReducer reducer({
      A::entry,
      B::entry,
      C::entry
    });

    CombinedSelector selector({
      &A::selector,
      &B::selector,
      &C::selector
    });

    BurpReduxTest::CombinedReducer::Entry entry = {
      field,
      &reducer,
      &selector
    };

  }

  CombinedReducer reducer({
    A::entry,
    B::entry
  });

  Store store(reducer, Store::Subscribers({
    &A::selector,
    &B::selector
  }));

  namespace A {
    namespace A {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::aa);
    }
    namespace B {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::ab);
    }
    namespace C {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::ac);
    }
  }

  namespace B {
    namespace A {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::ba);
    }
    namespace B {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::bb);
    }
    namespace C {
      BurpRedux::Dispatcher<State> dispatcher(store, creator, ReducerId::bc);
    }
  }

  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {

          StateList initialStates;
          initialStates.set(ReducerId::aa, A::A::creator.init("aa"));
          initialStates.set(ReducerId::ab, A::B::creator.init("ab"));
          initialStates.set(ReducerId::ac, A::C::creator.init("ac"));
          initialStates.set(ReducerId::ba, B::A::creator.init("ba"));
          initialStates.set(ReducerId::bb, B::B::creator.init("bb"));
          initialStates.set(ReducerId::bc, B::C::creator.init("bc"));
          store.init(initialStates);

          StaticJsonDocument<512> doc;
          doc[A::field][A::A::field][dataField] = 10;
          doc[A::field][A::B::field][dataField] = 20;
          doc[A::field][A::C::field][dataField] = 30;
          doc[B::field][B::A::field][dataField] = 40;
          doc[B::field][B::B::field][dataField] = 50;
          doc[B::field][B::C::field][dataField] = 60;
          store.getState()->deserialize(doc.as<JsonObject>());

      });

      describe.loop([]() {
          store.loop();
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(10, A::A::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->persistent);
          TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->persistent);
          TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->persistent);
          TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->persistent);
          TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bb", B::B::selector.getState()->persistent);
          TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->persistent);
      });

      describe.describe("then dispatch incrementData A::A", [](Describe & describe) {
          describe.before([](f_done & done) {
              A::A::subscriber.callbackOnce(done);
              const State * previous = A::A::selector.getState();
              using namespace std::placeholders;
              A::A::dispatcher.dispatch(previous, std::bind(&State::incrementData, _1, _2));
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
              TEST_ASSERT_EQUAL(11, A::A::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->persistent);

              TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
              TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->persistent);

              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
              TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
              TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::B::subscriber.count);
              TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bb", B::B::selector.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
              TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->persistent);
          });

          describe.describe("then dispatch setPersistent on B::B", [](Describe & describe) {
              describe.before([](f_done & done) {
                  B::B::subscriber.callbackOnce(done);
                  const State * previous = B::B::selector.getState();
                  using namespace std::placeholders;
                  B::B::dispatcher.dispatch(previous, std::bind(&State::setPersistent, _1, _2, "BB"));
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                  TEST_ASSERT_EQUAL(11, A::A::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
                  TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                  TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                  TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->persistent);

                  TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                  TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("BB", B::B::selector.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                  TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->persistent);
              });

              describe.describe("then serialize", [](Describe & describe) {
                  describe.it("should serialize the state", []() {
                      StaticJsonDocument<512> doc;
                      store.getState()->serialize(doc.to<JsonObject>());
                      TEST_ASSERT_EQUAL(11, doc[A::field][A::A::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(20, doc[A::field][A::B::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(30, doc[A::field][A::C::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(40, doc[B::field][B::A::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(50, doc[B::field][B::B::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(60, doc[B::field][B::C::field][dataField].as<int>());
                  });
              });
          });
      });

  });

}
