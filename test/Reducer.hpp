# pragma once

#include "../src/CppRedux/Reducer.hpp"
#include "Action.hpp"
#include "State.hpp"

namespace CppReduxTest {
  namespace Reducer {

    using ActionType = Action::ActionType;
    using Payload = Action::Payload;
    using Action = Action::Action;
    using State = State::State;

    class Reducer : public CppRedux::Reducer<State, Action> {

      public:

        const State * init(const State * previous, const State & initialState) const override {
          return update(previous, initialState);
        }

        const State * reduce(const State * previous, const Action & action) const override {
          switch (action.type) {
            case ActionType::ACTION:
              {
                State state(action.payload<Payload>()->data);
                return update(previous, state);
              }
            default:
              return previous;
          }
        }

    };

  }
}
