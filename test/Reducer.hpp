# pragma once

#include "../src/BurpRedux/Reducer.hpp"
#include "Action.hpp"

namespace BurpReduxTest {
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

    class Reducer : public BurpRedux::Reducer<State, Action> {

      public:

        const State * reduce(const State * previous, const Action & action) {
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
