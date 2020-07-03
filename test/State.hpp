# pragma once

#include "../src/BurpTree.hpp"

namespace BurpTreeTest {

  class State : public BurpTree::State {

    public: 

      const char * persistent;
      const int data;

      State(const char * persistent);
      State(const char * persistent, const int data);
      void serialize(const JsonObject & serialized) const override;

  };

  class Status : public BurpTree::Status {
    public:
      enum : BurpTree::Status::Code {
        ok,
        invalidData,
        noObject
      };
      const char * c_str() const override;
  };

  class Factory : public BurpTree::Factory<State, Status> {

    public:

      void setInitialPersistent(const char * persistent);

      bool deserialize(const JsonObject & serialized) override;
      bool createDefault() override;

      bool incrementData();
      bool setPersistent(const char * newPersistent);

    private:

      const char * _persistent;

  };

}
