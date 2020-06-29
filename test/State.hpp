# pragma once

#include "../src/BurpRedux/State/Instance.hpp"
#include "../src/BurpRedux/Creator/WithInitParams.hpp"
#include "../src/BurpRedux/Status.hpp"

namespace BurpReduxTest {

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

  class State : public BurpRedux::State::Instance {

    public: 

      int data;
      const char * persistent;

      State(const char * persistent, const Uid uid);
      State(const State * previous, const Uid uid);
      const Status & getStatus() const override;
      void serialize(const JsonObject & serialized) const override;
      void deserialize(const JsonObject & serialized) override;

      void incrementData(const State * previous);
      void setPersistent(const State * previous, const char * newPersistent);
      void setError(const State * previous);

    private:

      Status _status;

  };

  using Creator = BurpRedux::Creator::WithInitParams<State, const char *>;

}
