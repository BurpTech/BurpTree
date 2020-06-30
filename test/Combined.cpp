#include <unity.h>
#include <functional>
#include "../src/BurpRedux/Store.hpp"
#include "../src/BurpRedux/Reducer/Combined.hpp"
#include "../src/BurpRedux/Reducer/Instance.hpp"
#include "Constants.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Combined.hpp"

namespace BurpReduxTest {

  using Store = BurpRedux::Store;
  using Reducer1 = BurpRedux::Reducer::Combined<2, 1>;
  using Reducer2 = BurpRedux::Reducer::Combined<3, 1>;
  using Reducer3 = BurpRedux::Reducer::Instance<1>;

  namespace A {

    namespace A {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::aa, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    namespace B {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::ab, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    namespace C {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::ac, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    Subscriber subscriber;
    Reducer2 reducer(Reducer2::Map({
        A::entry,
        B::entry,
        C::entry
    }), Reducer2::Subscribers({
        &subscriber
    }));

    Reducer1::Entry entry = {
        field,
        &reducer
    };

  }

  namespace B {

    namespace A {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::ba, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    namespace B {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::bb, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    namespace C {
      StateFactory factory;
      Subscriber subscriber;
      Reducer3 reducer(ReducerId::bc, factory, Reducer3::Subscribers{
          &subscriber
      });
      Reducer2::Entry entry = {
          field,
          &reducer
      };
    }

    Subscriber subscriber;
    Reducer2 reducer(Reducer2::Map({
      A::entry,
      B::entry,
      C::entry
    }), Reducer2::Subscribers({
        &subscriber
    }));

    Reducer1::Entry entry = {
        field,
        &reducer
    };

  }

  Subscriber subscriber;
  Reducer1 reducer(Reducer1::Map({
      A::entry,
      B::entry
  }), Reducer1::Subscribers({
      &subscriber
  }));

  Store store(reducer);

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
          TEST_ASSERT_EQUAL(10, A::A::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("aa", A::A::reducer.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(20, A::B::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ab", A::B::reducer.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(30, A::C::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ac", A::C::reducer.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(40, B::A::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ba", B::A::reducer.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(50, B::B::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("bb", B::B::reducer.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(60, B::C::reducer.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("bc", B::C::reducer.getState()->as<State>()->persistent);
      });

      describe.describe("then dispatch incrementData A::A", [](Describe & describe) {
          describe.before([](f_done & done) {
              A::A::subscriber.callbackOnce(done);
              dispatch(ReducerId::aa, A::A::factory.incrementData());
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, subscriber.count);
              TEST_ASSERT_EQUAL(subscriber.state, reducer.getState());

              TEST_ASSERT_EQUAL(1, A::subscriber.count);
              TEST_ASSERT_EQUAL(A::subscriber.state, A::reducer.getState());

              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
              TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::reducer.getState());
              TEST_ASSERT_EQUAL(11, A::A::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("aa", A::A::reducer.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
              TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::reducer.getState());
              TEST_ASSERT_EQUAL(20, A::B::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ab", A::B::reducer.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
              TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::reducer.getState());
              TEST_ASSERT_EQUAL(30, A::C::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ac", A::C::reducer.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::subscriber.count);
              TEST_ASSERT_EQUAL(B::subscriber.state, B::reducer.getState());

              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
              TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::reducer.getState());
              TEST_ASSERT_EQUAL(40, B::A::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ba", B::A::reducer.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::B::subscriber.count);
              TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::reducer.getState());
              TEST_ASSERT_EQUAL(50, B::B::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("bb", B::B::reducer.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
              TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::reducer.getState());
              TEST_ASSERT_EQUAL(60, B::C::reducer.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("bc", B::C::reducer.getState()->as<State>()->persistent);
          });

          describe.describe("then dispatch setPersistent on B::B", [](Describe & describe) {
              describe.before([](f_done & done) {
                  B::B::subscriber.callbackOnce(done);
                  dispatch(ReducerId::bb, B::B::factory.setPersistent("BB"));
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(2, subscriber.count);
                  TEST_ASSERT_EQUAL(subscriber.state, reducer.getState());

                  TEST_ASSERT_EQUAL(1, A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::subscriber.state, A::reducer.getState());

                  TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::reducer.getState());
                  TEST_ASSERT_EQUAL(11, A::A::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("aa", A::A::reducer.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
                  TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::reducer.getState());
                  TEST_ASSERT_EQUAL(20, A::B::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ab", A::B::reducer.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                  TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::reducer.getState());
                  TEST_ASSERT_EQUAL(30, A::C::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ac", A::C::reducer.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(1, B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::subscriber.state, B::reducer.getState());

                  TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                  TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::reducer.getState());
                  TEST_ASSERT_EQUAL(40, B::A::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ba", B::A::reducer.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::reducer.getState());
                  TEST_ASSERT_EQUAL(50, B::B::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("BB", B::B::reducer.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                  TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::reducer.getState());
                  TEST_ASSERT_EQUAL(60, B::C::reducer.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("bc", B::C::reducer.getState()->as<State>()->persistent);
              });

              describe.describe("then serialize", [](Describe & describe) {
                  describe.it("should serialize the state", []() {
                      StaticJsonDocument<512> doc;
                      reducer.getState()->serialize(doc.to<JsonObject>());
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
