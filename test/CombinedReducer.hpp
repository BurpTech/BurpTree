#pragma once

#include <BddUnity.hpp>
#include "../src/CppRedux/CombinedReducer.hpp"
#include "Action.hpp"
#include "State.hpp"
#include "Reducer.hpp"

namespace CppReduxTest {
  namespace CombinedReducer {

    using ActionType = Action::ActionType;
    using Payload = Action::Payload;
    using Action = Action::Action;
    using State = State::State;
    using Reducer = Reducer::Reducer;

    class CombinedState : public CppRedux::State {

      public:

        const CombinedReducer::State * first;
        const CombinedReducer::State * second;

        CombinedState(const CombinedState * previous, const CombinedReducer::State * first, const CombinedReducer::State * second) :
          CppRedux::State(previous),
          first(first),
          second(second)
        {}

        CombinedState(const CombinedState * previous, const CombinedState & next) :
          CombinedState(previous, next.first, next.second)
        {}

        CombinedState(const CombinedReducer::State * first, const CombinedReducer::State * second) :
          CombinedState(nullptr, first, second)
        {}

    };

    class ReducerMap : public CppRedux::ReducerMap<CombinedState, Action> {

      public:

        ReducerMap(const Reducer firstReducer, const Reducer secondReducer) :
          _firstReducer(firstReducer),
          _secondReducer(secondReducer)
        {}

        const CombinedState * init(const CombinedState * previous, const CombinedState & initialState, f_update update) const override {
          const CombinedState state(
            _firstReducer.init(previous ? previous->first : nullptr, *(initialState.first)),
            _secondReducer.init(previous ? previous->second : nullptr, *(initialState.second))
          );
          return update(state);
        }

        const CombinedState * reduce(const CombinedState * previous, const Action & action, f_update update) const override {
          const CombinedState state(
            _firstReducer.reduce(previous ? previous->first : nullptr, action),
            _secondReducer.reduce(previous ? previous->second : nullptr, action)
          );
          return update(state);
        }

      private:

        const Reducer _firstReducer;
        const Reducer _secondReducer;

    };

    extern Module tests;

  }
}
