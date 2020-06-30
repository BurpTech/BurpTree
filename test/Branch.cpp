#include <unity.h>
#include <functional>
#include "../src/BurpTree.hpp"
#include "Constants.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Branch.hpp"

namespace BurpTreeTest {

  using Root = BurpTree::Root;
  using Node1 = BurpTree::Branch<2, 1>;
  using Node2 = BurpTree::Branch<3, 1>;
  using Node3 = BurpTree::Leaf<1>;

  namespace A {

    namespace A {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::aa, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    namespace B {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::ab, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    namespace C {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::ac, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    Subscriber subscriber;
    Node2 node(Node2::Map({
        A::entry,
        B::entry,
        C::entry
    }), Node2::Subscribers({
        &subscriber
    }));

    Node1::Entry entry = {
        field,
        &node
    };

  }

  namespace B {

    namespace A {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::ba, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    namespace B {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::bb, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    namespace C {
      StateFactory factory;
      Subscriber subscriber;
      Node3 node(NodeId::bc, factory, Node3::Subscribers{
          &subscriber
      });
      Node2::Entry entry = {
          field,
          &node
      };
    }

    Subscriber subscriber;
    Node2 node(Node2::Map({
      A::entry,
      B::entry,
      C::entry
    }), Node2::Subscribers({
        &subscriber
    }));

    Node1::Entry entry = {
        field,
        &node
    };

  }

  Subscriber subscriber;
  Node1 node(Node1::Map({
      A::entry,
      B::entry
  }), Node1::Subscribers({
      &subscriber
  }));

  Root root(node);

  void dispatch(const BurpTree::Node::Id id, const BurpTree::State * state) {
    if (state) {
      root.dispatch(id, state);
    }
  }

  Module branchTests("Branch", [](Describe & describe) {

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

          root.deserialize(doc.as<JsonObject>());

      });

      describe.loop([]() {
          root.loop();
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(10, A::A::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(20, A::B::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(30, A::C::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(40, B::A::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(50, B::B::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("bb", B::B::node.getState()->as<State>()->persistent);
          TEST_ASSERT_EQUAL(60, B::C::node.getState()->as<State>()->data);
          TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->as<State>()->persistent);
      });

      describe.describe("then dispatch incrementData A::A", [](Describe & describe) {
          describe.before([](f_done & done) {
              A::A::subscriber.callbackOnce(done);
              dispatch(NodeId::aa, A::A::factory.incrementData());
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, subscriber.count);
              TEST_ASSERT_EQUAL(subscriber.state, node.getState());

              TEST_ASSERT_EQUAL(1, A::subscriber.count);
              TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
              TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
              TEST_ASSERT_EQUAL(11, A::A::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
              TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
              TEST_ASSERT_EQUAL(20, A::B::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
              TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
              TEST_ASSERT_EQUAL(30, A::C::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::subscriber.count);
              TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
              TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
              TEST_ASSERT_EQUAL(40, B::A::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::B::subscriber.count);
              TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
              TEST_ASSERT_EQUAL(50, B::B::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("bb", B::B::node.getState()->as<State>()->persistent);

              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
              TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
              TEST_ASSERT_EQUAL(60, B::C::node.getState()->as<State>()->data);
              TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->as<State>()->persistent);
          });

          describe.describe("then dispatch setPersistent on B::B", [](Describe & describe) {
              describe.before([](f_done & done) {
                  B::B::subscriber.callbackOnce(done);
                  dispatch(NodeId::bb, B::B::factory.setPersistent("BB"));
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(2, subscriber.count);
                  TEST_ASSERT_EQUAL(subscriber.state, node.getState());

                  TEST_ASSERT_EQUAL(1, A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

                  TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
                  TEST_ASSERT_EQUAL(11, A::A::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
                  TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
                  TEST_ASSERT_EQUAL(20, A::B::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                  TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
                  TEST_ASSERT_EQUAL(30, A::C::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(1, B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

                  TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                  TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
                  TEST_ASSERT_EQUAL(40, B::A::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
                  TEST_ASSERT_EQUAL(50, B::B::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("BB", B::B::node.getState()->as<State>()->persistent);

                  TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                  TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
                  TEST_ASSERT_EQUAL(60, B::C::node.getState()->as<State>()->data);
                  TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->as<State>()->persistent);
              });

              describe.describe("then serialize", [](Describe & describe) {
                  describe.it("should serialize the state", []() {
                      StaticJsonDocument<512> doc;
                      node.getState()->serialize(doc.to<JsonObject>());
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
