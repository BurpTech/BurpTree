# pragma once

#include "../src/BurpRedux/Reducer.hpp"
#include "State.hpp"
#include "Action.hpp"

namespace BurpReduxTest {

  class Reducer : public BurpRedux::Reducer<State, Action> {

    public:

      const State * reduce(const State * previous, const Action & action) {
        switch (action.type) {
          case ActionType::ACTION:
            {
              const Params params = {
                action.payload<Payload>()->data,
                previous->data2
              };
              State * next = new State(&params);
              delete previous;
              return next;
            }
          default:
            return previous;
        }
      }

  };

}
