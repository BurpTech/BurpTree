#pragma once

#include "../Combined.hpp"
#include "Pool.hpp"

namespace BurpRedux {
  namespace State {
    namespace Factory {

      template <size_t length>
      class Combined : public Pool<State::Combined<length>> {

        public:

          using State = State::Combined<length>;
          using Uid = Interface::Uid;
          using Fields = typename State::Fields;
          using States = typename State::States;

          Combined(const Fields & fields) :
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
