#pragma once

#include <functional>
#include "../Reducer/Interface.hpp"
#include "../State/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class CombinedParams, class State>
    class Instance : public Interface<CombinedState, CombinedParams> {

      public:

        using f_get = std::function<const State * (const CombinedState * state)>;
        using f_set = std::function<void(CombinedParams & params, const State * state)>;

        Instance(f_get get, f_set set, Reducer::Interface<State> & reducer) :
          _get(get),
          _set(set),
          _reducer(reducer)
        {}

        bool reduce(const CombinedState * state, CombinedParams & params, const Action::Interface & action) override {
          const State * current = _get(state);
          // we do this cast so that the compiler will
          // check that the user knows what they're doing.
          // States must implement the State base class and
          // expose a getUid method so that we can safely(?)
          // check if a state has (eally changed
          const BurpRedux::State::Interface * currentBase = current;
          unsigned long uid = currentBase->getUid();
          const State * next = _reducer.reduce(current, action);
          _set(params, next);
          const BurpRedux::State::Interface * nextBase = next;
          return nextBase->getUid() != uid;
        }

      private:

        f_get _get;
        f_set _set;
        Reducer::Interface<State> & _reducer;

    };

  }
}
