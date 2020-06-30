# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/State/Factory/Deserializer.hpp"
#include "../src/BurpRedux/State/Factory/Pool.hpp"
#include "../src/BurpRedux/Status.hpp"
#include <functional>

namespace BurpReduxTest {

  class State : public BurpRedux::State::Instance {

    public: 

      const char * persistent;
      const int data;

      State(const Uid uid, const char * persistent, const int data);
      void serialize(const JsonObject & serialized) const override;

  };

  class Status : public BurpRedux::Status {

    public:

      using Level = BurpRedux::Status::Level;

      enum Code : BurpRedux::Status::Code {
        noError,
        error
      };

      const char * c_str() const override {
        switch (getCode()) {
          case noError:
            return "BurpReduxTest::State : no error";
          case error:
            return "BurpReduxTest::State : error";
          default:
            return "BurpReduxTest::State : unknown";
        }
      }

  };

  class StateFactory : public BurpRedux::State::Factory::Deserializer, public BurpRedux::State::Factory::Pool<State> {

    public:

      void setInitialPersistent(const char * persistent);
      const Status & getStatus() const;

      const BurpRedux::State::Interface * deserialize(const JsonObject & serialized) override ;
      const BurpRedux::State::Interface * incrementData();
      const BurpRedux::State::Interface * setPersistent(const char * newPersistent);
      const BurpRedux::State::Interface * setError();

    private:

      const char * _persistent;
      Status _status;

  };

}
