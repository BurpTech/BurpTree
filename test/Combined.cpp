#include <unity.h>
#include "../src/BurpRedux.hpp"
#include "Action.hpp"
#include "Constants.hpp"
#include "Actions.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Combined.hpp"

namespace BurpReduxTest {

  namespace A {

    namespace A {
      Creator creator;
      Reducer reducer(ReducerId::aa);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::a, Selector::Subscribers({
          &subscriber
      }));
    }

    namespace B {
      Creator creator;
      Reducer reducer(ReducerId::ab);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::b, Selector::Subscribers({
          &subscriber
      }));
    }

    namespace C {
      Creator creator;
      Reducer reducer(ReducerId::ac);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::c, Selector::Subscribers({
          &subscriber
      }));
    }

    using Reducer = BurpRedux::CombinedReducer<ReducerIndex::count>;
    Reducer::Map map;
    map[ReducerIndex::a] = {A::field, &A::reducer};
    map[ReducerIndex::b] = {B::field, &B::reducer};
    map[ReducerIndex::c] = {C::field, &C::reducer};
    Reducer reducer(map);

    using Selector = BurpRedux::Selector::Instance<3>;
    Selector selector(BurpReduxTest::ReducerIndex::a, Selector::Subscribers({
      &A::selector,
      &B::selector,
      &C::selector
    }));

  }

  namespace B {

    namespace A {
      Creator creator;
      Reducer reducer(ReducerId::ba);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::a, Selector::Subscribers({
          &subscriber
      }));
    }

    namespace B {
      Creator creator;
      Reducer reducer(ReducerId::bb);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::b, Selector::Subscribers({
          &subscriber
      }));
    }

    namespace C {
      Creator creator;
      Reducer reducer(ReducerId::bc);
      Subscriber subscriber;
      using Selector = BurpRedux::Selector::Instance<1>;
      Selector selector(ReducerIndex::c, Selector::Subscribers({
          &subscriber
      }));
    }

    using Reducer = BurpRedux::CombinedReducer<ReducerIndex::count>;
    Reducer::Map map;
    map[ReducerIndex::a] = {A::field, &A::reducer};
    map[ReducerIndex::b] = {B::field, &B::reducer};
    map[ReducerIndex::c] = {C::field, &C::reducer};
    Reducer reducer(map);

    using Selector = BurpRedux::Selector::Instance<3>;
    Selector selector(BurpReduxTest::ReducerIndex::b, Selector::Subscribers({
      &A::selector,
      &B::selector,
      &C::selector
    }));

  }

  using Reducer = BurpRedux::CombinedReducer<ReducerIndex::count>;
  Reducer::Map map;
  map[ReducerIndex::a] = {A::field, &A::reducer};
  map[ReducerIndex::b] = {A::field, &B::reducer};
  Reducer reducer(map);

  using Store = BurpRedux::Store::Instance<ReducerId::count, 2>;
  Store store(reducer, Store::Subscribers({
    &A::selector,
    &B::selector
  }));

  namespace A {
    namespace A {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::aa);
    }
    namespace B {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::ab);
    }
    namespace C {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::ac);
    }
  }

  namespace B {
    namespace A {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::ba);
    }
    namespace B {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::bb);
    }
    namespace C {
      Action::Dispatcher<State, Params> dispatcher(store, creator, ReducerId::bc);
    }
  }

  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {

          Store::InitialStates initialStates;
          initialStates[ReducerId::aa] = A::A::creator.init({"aa"});
          initialStates[ReducerId::ab] = A::B::creator.init({"ab"});
          initialStates[ReducerId::ac] = A::C::creator.init({"ac"});
          initialStates[ReducerId::ba] = B::A::creator.init({"ba"});
          initialStates[ReducerId::bb] = B::B::creator.init({"bb"});
          initialStates[ReducerId::bc] = B::C::creator.init({"bc"});
          store.init(initialStates);

          StaticJsonDocument<512> doc;
          doc[A::field][A::A::field][dataField] = 10;
          doc[A::field][A::B::field][dataField] = 20;
          doc[A::field][A::C::field][dataField] = 30;
          doc[B::field][B::A::field][dataField] = 40;
          doc[B::field][B::B::field][dataField] = 50;
          doc[B::field][B::C::field][dataField] = 60;
          store.deserialize(doc.as<JsonObject>());

      });

      describe.loop([]() {
          store.loop();
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(10, A::A::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->external);
          TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->external);
          TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->external);
          TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->external);
          TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bb", B::B::selector.getState()->external);
          TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->external);
      });

      describe.describe("then dispatch incrementData A::A", [](Describe & describe) {
          describe.before([](f_done & done) {
              A::A::subscriber.callbackOnce(done);
              const State * previous = A::A::selector.getState();
              using std::placeholders;
              A::A::dispatcher.dispatch(previous, std::bind(Action::incrementData, _1, _2));
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
              TEST_ASSERT_EQUAL(11, A::A::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->external);

              TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
              TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->external);

              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
              TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->external);

              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
              TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->external);

              TEST_ASSERT_EQUAL(0, B::B::subscriber.count);
              TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bb", B::B::selector.getState()->external);

              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
              TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->external);
          });

          describe.describe("then dispatch setExternal on B::B", [](Describe & describe) {
              describe.before([](f_done & done) {
                  B::B::subscriber.callbackOnce(done);
                  const State * previous = B::B::selector.getState();
                  using std::placeholders;
                  B::B::dispatcher.dispatch(previous, std::bind(Action::setExternal, _1, _2, "BB"));
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                  TEST_ASSERT_EQUAL(11, A::A::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("aa", A::A::selector.getState()->external);

                  TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
                  TEST_ASSERT_EQUAL(20, A::B::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ab", A::B::selector.getState()->external);

                  TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                  TEST_ASSERT_EQUAL(30, A::C::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ac", A::C::selector.getState()->external);

                  TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                  TEST_ASSERT_EQUAL(40, B::A::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ba", B::A::selector.getState()->external);

                  TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                  TEST_ASSERT_EQUAL(50, B::B::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("BB", B::B::selector.getState()->external);

                  TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                  TEST_ASSERT_EQUAL(60, B::C::selector.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("bc", B::C::selector.getState()->external);
              });

              describe.describe("then serialize", [](Describe & describe) {
                  describe.it("should serialize the state", []() {
                      StaticJsonDocument<512> doc;
                      store.getState()->serialize(doc.to<JsonObject>());
                      TEST_ASSERT_EQUAL(11, doc[A:field][A::A::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(20, doc[A:field][A::B::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(30, doc[A:field][A::C::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(40, doc[B:field][B::A::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(51, doc[B:field][B::B::field][dataField].as<int>());
                      TEST_ASSERT_EQUAL(60, doc[B:field][B::C::field][dataField].as<int>());
                  });
              });
          });
      });

  });

}
