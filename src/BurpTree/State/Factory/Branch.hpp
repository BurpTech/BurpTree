#pragma once

#include "../Branch.hpp"
#include "Pool.hpp"

namespace BurpTree {
  namespace State {
    namespace Factory {

      template <size_t length>
      class Branch : public Pool<State::Branch<length>> {

        public:

          using State = State::Branch<length>;
          using Uid = Interface::Uid;
          using Fields = typename State::Fields;
          using States = typename State::States;

          Branch(const Fields & fields) :
            _fields(fields)
          {}

          const Interface * create(const States & states) {
            return Pool<State>::_create([&](const Uid uid, void * address) {
                return new(address) State(uid, _fields, states);
            });
          }

        private:
          
          const Fields & _fields;

      };

    }
  }
}
