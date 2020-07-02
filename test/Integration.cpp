#include <unity.h>
#include <functional>
#include "../src/BurpTree.hpp"
#include "Constants.hpp"
#include "State.hpp"
#include "Subscriber.hpp"
#include "Integration.hpp"

namespace BurpTreeTest {

  using Node1 = BurpTree::Branch<2, 1>;
  using Node2 = BurpTree::Branch<3, 1>;
  using Node3 = BurpTree::Leaf<Factory, 1>;
  using Dispatcher = BurpTree::Dispatcher<Factory>;
  using Root = BurpTree::Root<Node1>;

  namespace A {

    namespace A {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::aa, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    namespace B {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::ab, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    namespace C {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::ac, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    Subscriber<Node2::State> subscriber;
    Node2 node(Node2::Map({
        Node2::Entry({A::field, &A::node}),
        Node2::Entry({B::field, &B::node}),
        Node2::Entry({C::field, &C::node}),
    }), Node2::Subscribers({
        &subscriber
    }));

  }

  namespace B {

    namespace A {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::ba, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    namespace B {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::bb, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    namespace C {
      Subscriber<State> subscriber;
      Factory factory;
      Node3 node(NodeId::bc, factory, Node3::Subscribers({
          &subscriber
      }));
    }

    Subscriber<Node2::State> subscriber;
    Node2 node(Node2::Map({
      Node2::Entry({A::field, &A::node}),
      Node2::Entry({B::field, &B::node}),
      Node2::Entry({C::field, &C::node}),
    }), Node2::Subscribers({
        &subscriber
    }));

  }

  Subscriber<Node1::State> subscriber;
  Node1 node(Node1::Map({
      Node1::Entry({A::field, &A::node}),
      Node1::Entry({B::field, &B::node})
  }), Node1::Subscribers({
      &subscriber
  }));

  Root root(node);

  namespace A {
    namespace A {
      Dispatcher dispatcher(root, node);
    }
    namespace B {
      Dispatcher dispatcher(root, node);
    }
    namespace C {
      Dispatcher dispatcher(root, node);
    }
  }

  namespace B {
    namespace A {
      Dispatcher dispatcher(root, node);
    }
    namespace B {
      Dispatcher dispatcher(root, node);
    }
    namespace C {
      Dispatcher dispatcher(root, node);
    }
  }

  Module integrationTests("Integration", [](Describe & describe) {

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
          // "hello" is invalid data and should result in the default value
          // on initial deserialization
          doc[B::field][B::B::field][dataField] = "hello";
          // Not setting a value should also result in the default value
          // on initial deserialization
          //doc[B::field][B::C::field][dataField] = 60;

          root.deserialize(doc.as<JsonObject>());

      });

      describe.loop([]() {
          root.loop();
      });

      describe.it("should set the correct statuses on the factories", []() {
          const BurpTree::Status statusAA = A::A::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusAA.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusAA.getCode());

          const BurpTree::Status statusAB = A::B::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusAB.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusAB.getCode());

          const BurpTree::Status statusAC = A::C::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusAC.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusAC.getCode());

          const BurpTree::Status statusBA = B::A::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusBA.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusBA.getCode());

          const BurpTree::Status statusBB = B::B::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusBB.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusBB.getCode());

          const BurpTree::Status statusBC = B::C::factory.getStatus();
          TEST_ASSERT_EQUAL(Status::Level::INFO, statusBC.getLevel());
          TEST_ASSERT_EQUAL(Status::ok, statusBC.getCode());
      });

      describe.it("should have the correct initial states", []() {
          TEST_ASSERT_EQUAL(10, A::A::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);
          TEST_ASSERT_EQUAL(20, A::B::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);
          TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);
          TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);
          TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bb", B::B::node.getState()->persistent);
          TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
          TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
      });

      describe.describe("then dispatch incrementData A::A", [](Describe & describe) {
          describe.before([](f_done & done) {
              A::A::subscriber.callbackOnce(done);
              A::A::dispatcher.dispatch(&Factory::incrementData);
          });

          describe.it("should notify the correct subscribers with the correct state", []() {
              TEST_ASSERT_EQUAL(1, subscriber.count);
              TEST_ASSERT_EQUAL(subscriber.state, node.getState());

              TEST_ASSERT_EQUAL(1, A::subscriber.count);
              TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
              TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
              TEST_ASSERT_EQUAL(11, A::A::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);

              TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
              TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
              TEST_ASSERT_EQUAL(20, A::B::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);

              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
              TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
              TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::subscriber.count);
              TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
              TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
              TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::B::subscriber.count);
              TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
              TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bb", B::B::node.getState()->persistent);

              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
              TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
              TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
              TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
          });

          describe.describe("then dispatch setPersistent on B::B", [](Describe & describe) {
              describe.before([](f_done & done) {
                  B::B::subscriber.callbackOnce(done);
                  using namespace std::placeholders;
                  B::B::dispatcher.dispatch(std::bind(&Factory::setPersistent, _1, "BB"));
              });

              describe.it("should notify the correct subscribers with the correct state", []() {
                  TEST_ASSERT_EQUAL(2, subscriber.count);
                  TEST_ASSERT_EQUAL(subscriber.state, node.getState());

                  TEST_ASSERT_EQUAL(1, A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

                  TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                  TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
                  TEST_ASSERT_EQUAL(11, A::A::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, A::B::subscriber.count);
                  TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
                  TEST_ASSERT_EQUAL(20, A::B::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                  TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
                  TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);

                  TEST_ASSERT_EQUAL(1, B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

                  TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                  TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
                  TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);

                  TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                  TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
                  TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("BB", B::B::node.getState()->persistent);

                  TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                  TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
                  TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
                  TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
              });

              describe.describe("then dispatch deserialize on A::B", [](Describe & describe) {
                  describe.before([](f_done & done) {
                      A::B::subscriber.callbackOnce(done);
                      StaticJsonDocument<512> doc;
                      doc[dataField] = 25;
                      using namespace std::placeholders;
                      A::B::dispatcher.dispatch(std::bind(&Factory::deserialize, _1, doc.as<JsonObject>()));
                  });

                  describe.it("should notify the correct subscribers with the correct state", []() {
                      TEST_ASSERT_EQUAL(3, subscriber.count);
                      TEST_ASSERT_EQUAL(subscriber.state, node.getState());

                      TEST_ASSERT_EQUAL(2, A::subscriber.count);
                      TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

                      TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                      TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
                      TEST_ASSERT_EQUAL(11, A::A::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);

                      TEST_ASSERT_EQUAL(1, A::B::subscriber.count);
                      TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
                      TEST_ASSERT_EQUAL(25, A::B::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);

                      TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                      TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
                      TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);

                      TEST_ASSERT_EQUAL(1, B::subscriber.count);
                      TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

                      TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                      TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
                      TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);

                      TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                      TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
                      TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("BB", B::B::node.getState()->persistent);

                      TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                      TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
                      TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
                      TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
                  });

                  describe.describe("then dispatch invalid deserialize on A::B", [](Describe & describe) {
                      describe.it("should return an invalidData status", []() {
                          StaticJsonDocument<512> doc;
                          doc[dataField] = "hello";
                          using namespace std::placeholders;
                          const BurpTree::Status status = A::B::dispatcher.dispatch(std::bind(&Factory::deserialize, _1, doc.as<JsonObject>()));
                          TEST_ASSERT_EQUAL(Status::Level::ERROR, status.getLevel());
                          TEST_ASSERT_EQUAL(Status::invalidData, status.getCode());
                      });

                      describe.it("should not update anything", []() {
                          TEST_ASSERT_EQUAL(3, subscriber.count);
                          TEST_ASSERT_EQUAL(subscriber.state, node.getState());

                          TEST_ASSERT_EQUAL(2, A::subscriber.count);
                          TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

                          TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                          TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
                          TEST_ASSERT_EQUAL(11, A::A::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);

                          TEST_ASSERT_EQUAL(1, A::B::subscriber.count);
                          TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
                          TEST_ASSERT_EQUAL(25, A::B::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);

                          TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                          TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
                          TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);

                          TEST_ASSERT_EQUAL(1, B::subscriber.count);
                          TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

                          TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                          TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
                          TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);

                          TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                          TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
                          TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("BB", B::B::node.getState()->persistent);

                          TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                          TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
                          TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
                          TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
                      });

                      describe.describe("then dispatch empty deserialize on A::B", [](Describe & describe) {
                          describe.it("should return a noObject status", []() {
                              StaticJsonDocument<512> doc;
                              using namespace std::placeholders;
                              const BurpTree::Status status = A::B::dispatcher.dispatch(std::bind(&Factory::deserialize, _1, doc.as<JsonObject>()));
                              TEST_ASSERT_EQUAL(Status::Level::ERROR, status.getLevel());
                              TEST_ASSERT_EQUAL(Status::noObject, status.getCode());
                          });

                          describe.it("should not update anything", []() {
                              TEST_ASSERT_EQUAL(3, subscriber.count);
                              TEST_ASSERT_EQUAL(subscriber.state, node.getState());

                              TEST_ASSERT_EQUAL(2, A::subscriber.count);
                              TEST_ASSERT_EQUAL(A::subscriber.state, A::node.getState());

                              TEST_ASSERT_EQUAL(1, A::A::subscriber.count);
                              TEST_ASSERT_EQUAL(A::A::subscriber.state, A::A::node.getState());
                              TEST_ASSERT_EQUAL(11, A::A::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("aa", A::A::node.getState()->persistent);

                              TEST_ASSERT_EQUAL(1, A::B::subscriber.count);
                              TEST_ASSERT_EQUAL(A::B::subscriber.state, A::B::node.getState());
                              TEST_ASSERT_EQUAL(25, A::B::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("ab", A::B::node.getState()->persistent);

                              TEST_ASSERT_EQUAL(0, A::C::subscriber.count);
                              TEST_ASSERT_EQUAL(A::C::subscriber.state, A::C::node.getState());
                              TEST_ASSERT_EQUAL(30, A::C::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("ac", A::C::node.getState()->persistent);

                              TEST_ASSERT_EQUAL(1, B::subscriber.count);
                              TEST_ASSERT_EQUAL(B::subscriber.state, B::node.getState());

                              TEST_ASSERT_EQUAL(0, B::A::subscriber.count);
                              TEST_ASSERT_EQUAL(B::A::subscriber.state, B::A::node.getState());
                              TEST_ASSERT_EQUAL(40, B::A::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("ba", B::A::node.getState()->persistent);

                              TEST_ASSERT_EQUAL(1, B::B::subscriber.count);
                              TEST_ASSERT_EQUAL(B::B::subscriber.state, B::B::node.getState());
                              TEST_ASSERT_EQUAL(0, B::B::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("BB", B::B::node.getState()->persistent);

                              TEST_ASSERT_EQUAL(0, B::C::subscriber.count);
                              TEST_ASSERT_EQUAL(B::C::subscriber.state, B::C::node.getState());
                              TEST_ASSERT_EQUAL(0, B::C::node.getState()->data);
                              TEST_ASSERT_EQUAL_STRING("bc", B::C::node.getState()->persistent);
                          });

                          describe.describe("then serialize", [](Describe & describe) {
                              describe.it("should serialize the state", []() {
                                  StaticJsonDocument<512> doc;
                                  node.getState()->serialize(doc.to<JsonObject>());
                                  TEST_ASSERT_EQUAL(11, doc[A::field][A::A::field][dataField].as<int>());
                                  TEST_ASSERT_EQUAL(25, doc[A::field][A::B::field][dataField].as<int>());
                                  TEST_ASSERT_EQUAL(30, doc[A::field][A::C::field][dataField].as<int>());
                                  TEST_ASSERT_EQUAL(40, doc[B::field][B::A::field][dataField].as<int>());
                                  TEST_ASSERT_EQUAL(0, doc[B::field][B::B::field][dataField].as<int>());
                                  TEST_ASSERT_EQUAL(0, doc[B::field][B::C::field][dataField].as<int>());
                              });
                          });
                      });
                  });
              });
          });
      });

  });

}
