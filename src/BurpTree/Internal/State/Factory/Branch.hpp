#pragma once

#include "../Branch/Instance.hpp"
#include "Pool.hpp"
#include "../../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <size_t length>
        class Branch : public Pool<State::Branch::Instance<length>, Status> {

          public:

            using BranchState = State::Branch::Instance<length>;
            using Fields = typename BranchState::Fields;
            using States = typename BranchState::States;
            using Uid = Base::Uid;
            using P = Pool<BranchState, Status>;

            Branch(const Fields & fields) :
              _fields(fields)
            {}

            const Base * create(const States & states) {
              return P::create([&]() -> const BranchState * {
                  return new(this->getAddress()) BranchState(this->getUid(), _fields, states);
              });
            }

          private:
            
            const Fields & _fields;

            const BranchState * _default() override {
              // Should never get here as we never call Pool::fail()
              return nullptr;
            }

        };

      }
    }
  }
}
