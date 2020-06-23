# pragma once

#include "../src/BurpRedux/Reducer/Interface.hpp"
#include "State.hpp"
#include "Action.hpp"

namespace BurpReduxTest {

  template <ActionType type>
  class Reducer : public BurpRedux::Reducer::Interface<State, Action> {

    public:

      const State * reduce(const State * previous, const Action & action) override {
        switch (action.type) {
          case type:
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

  extern Reducer<ActionType::ACTION> reducer;

}
