# pragma once

#include "Action.hpp"
#include "../src/CppRedux/Reducer.hpp"

namespace CppReduxTest {
  namespace Reducer {

    using ActionType = Action::ActionType;
    using Payload = Action::Payload;
    using Action = Action::Action;

    class State {

      public: 

        int data1;
        int data2;

        State(int data1, int data2) :
          data1(data1),
          data2(data2)
        {}

    };

    class Reducer : public CppRedux::Reducer<State, Action> {

      public:

        const State * reduce(const State * previous, const Action & action) const override {
          switch (action.type) {
            case ActionType::ACTION:
              {
                State * next = new State(
                  action.payload<Payload>()->data,
                  previous->data2
                );
                delete previous;
                return next;
              }
            default:
              return previous;
          }
        }

    };

  }
}
