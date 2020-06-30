# pragma once

#include "../src/BurpTree.hpp"

namespace BurpTreeTest {

  class State : public BurpTree::State {

    public: 

      const char * persistent;
      const int data;

      State(const Uid uid, const char * persistent, const int data);
      void serialize(const JsonObject & serialized) const override;

  };

  class StateFactory : public BurpTree::Factory<BurpTreeTest::State> {

    public:

      void setInitialPersistent(const char * persistent);

      const BurpTree::Status & getStatus() const override;
      const BurpTree::State * deserialize(const JsonObject & serialized) override ;

      const BurpTree::State * incrementData();
      const BurpTree::State * setPersistent(const char * newPersistent);
      const BurpTree::State * setError();

    private:

      class Status : public BurpTree::Status {

        public:

          using Level = BurpTree::Status::Level;

          enum Code : BurpTree::Status::Code {
            noError,
            error
          };

          const char * c_str() const override {
            switch (getCode()) {
              case noError:
                return "BurpTreeTest::State : no error";
              case error:
                return "BurpTreeTest::State : error";
              default:
                return "BurpTreeTest::State : unknown";
            }
          }

      };

      const char * _persistent;
      Status _status;

  };

}
