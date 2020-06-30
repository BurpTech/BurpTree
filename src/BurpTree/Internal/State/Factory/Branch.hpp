#pragma once

#include "../Branch/Instance.hpp"
#include "Pool.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <size_t length>
        class Branch : public Pool<State::Branch::Instance<length>> {

          public:

            using BranchState = State::Branch::Instance<length>;
            using Fields = typename BranchState::Fields;
            using States = typename BranchState::States;
            using State = Base;
            using Uid = State::Uid;

            Branch(const Fields & fields) :
              _fields(fields)
            {}

            const State * create(const States & states) {
              return Pool<BranchState>::_create([&](const Uid uid, void * address) {
                  return new(address) BranchState(uid, _fields, states);
              });
            }

          private:
            
            const Fields & _fields;

        };

      }
    }
  }
}
