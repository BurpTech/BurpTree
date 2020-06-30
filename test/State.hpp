# pragma once

#include "../src/BurpTree/State/Instance.hpp"
#include "../src/BurpTree/State/Factory/Deserializer.hpp"
#include "../src/BurpTree/State/Factory/Pool.hpp"
#include "../src/BurpTree/Status.hpp"
#include <functional>

namespace BurpTreeTest {

  class State : public BurpTree::State::Instance {

    public: 

      const char * persistent;
      const int data;

      State(const Uid uid, const char * persistent, const int data);
      void serialize(const JsonObject & serialized) const override;

  };

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

  class StateFactory : public BurpTree::State::Factory::Deserializer, public BurpTree::State::Factory::Pool<State> {

    public:

      void setInitialPersistent(const char * persistent);
      const Status & getStatus() const;

      const BurpTree::State::Interface * deserialize(const JsonObject & serialized) override ;
      const BurpTree::State::Interface * incrementData();
      const BurpTree::State::Interface * setPersistent(const char * newPersistent);
      const BurpTree::State::Interface * setError();

    private:

      const char * _persistent;
      Status _status;

  };

}
