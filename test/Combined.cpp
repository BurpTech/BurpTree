#include <unity.h>
#include <functional>
#include "../src/BurpRedux/Store/Instance.hpp"
#include "../src/BurpRedux/Reducer/Combined.hpp"
#include "../src/BurpRedux/Reducer/Instance.hpp"
#include "../src/BurpRedux/Selector/Instance.hpp"
#include "Constants.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Combined.hpp"

using Reducer = BurpRedux::Reducer::Instance;

namespace BurpReduxTest {

  using CombinedReducer = BurpRedux::Reducer::Combined<2>;
  using Store = BurpRedux::Store::Instance<CombinedReducer::State, 2>;

  namespace A {

    using CombinedReducer = BurpRedux::Reducer::Combined<3>;
    using CombinedSelector = BurpRedux::Selector::Instance<BurpReduxTest::CombinedReducer::State, CombinedReducer::State, 3>;
    using Selector = BurpRedux::Selector::Instance<CombinedReducer::State, State, 1>;

    namespace A {
      StateFactory factory;
      Reducer reducer(ReducerId::aa, factory);
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
      StateFactory factory;
      Reducer reducer(ReducerId::ab, factory);
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
      StateFactory factory;
      Reducer reducer(ReducerId::ac, factory);
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

    using CombinedReducer = BurpRedux::Reducer::Combined<3>;
    using CombinedSelector = BurpRedux::Selector::Instance<BurpReduxTest::CombinedReducer::State, CombinedReducer::State, 3>;
    using Selector = BurpRedux::Selector::Instance<CombinedReducer::State, State, 1>;

    namespace A {
      StateFactory factory;
      Reducer reducer(ReducerId::ba, factory);
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
      StateFactory factory;
      Reducer reducer(ReducerId::bb, factory);
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
      StateFactory factory;
      Reducer reducer(ReducerId::bc, factory);
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

  void dispatch(const BurpRedux::Reducer::Interface::Id id, const BurpRedux::State::Interface * state) {
    if (state) {
      store.dispatch(id, state);
    }
  }

  Module combinedTests("Combined", [](Describe & describe) {

      describe.setup([]() {

          A::A::factory.setInitialPersistent("aa");
          A::B::factory.setInitialPersistent("ab");
          A::C::factory.setInitialPersistent("ac");
          B::A::factory.setInitialPersistent("ba");
          B::B::factory.setInitialPersistent("bb");
          B::C::factory.setInitialPersistent("bc");

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
              dispatch(ReducerId::aa, A::A::factory.incrementData());
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
                  dispatch(ReducerId::bb, B::B::factory.setPersistent("BB"));
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
