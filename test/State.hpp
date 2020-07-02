# pragma once

#include "../src/BurpTree.hpp"

namespace BurpTreeTest {

  class State : public BurpTree::State {

    public: 

      const char * persistent;
      const int data;

      State(const Uid uid, const char * persistent);
      State(const Uid uid, const char * persistent, const int data);
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

      const BurpTree::State * deserialize(const JsonObject & serialized) override ;
      const BurpTree::State * incrementData();
      const BurpTree::State * setPersistent(const char * newPersistent);

    private:

      const char * _persistent;

      const State * _default() override;

  };

}
