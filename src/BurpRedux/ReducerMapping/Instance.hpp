#pragma once

#include <functional>
#include "../Reducer/Interface.hpp"
#include "../State/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class State>
    using f_get = const State * (*)(const CombinedState * state);

    template <class CombinedParams, class State>
    using f_set = void (*)(CombinedParams & params, const State * state);

    template <
      class CombinedState,
      class CombinedParams,
      class State,
      const char * serializedField,
      f_get<CombinedState, State> get,
      f_set<CombinedParams, State> set
    >
    class Instance : public Interface<CombinedState, CombinedParams> {

      public:

        Instance(Reducer::Interface<State> & reducer) :
          _reducer(reducer)
        {}

        void deserialize(const JsonObject & serialized, CombinedParams & params) override {
          const JsonObject object = serialized[serializedField].as<JsonObject>();
          const State * initial = _reducer.deserialize(object);
          set(params, initial);
        }

        bool reduce(const CombinedState * state, CombinedParams & params, const Action::Interface & action) override {
          const State * current = get(state);
          // we do this cast so that the compiler will
          // check that the user knows what they're doing.
          // States must implement the State base class and
          // expose a getUid method so that we can safely(?)
          // check if a state has (eally changed
          const BurpRedux::State::Interface * currentBase = current;
          unsigned long uid = currentBase->getUid();
          const State * next = _reducer.reduce(current, action);
          set(params, next);
          const BurpRedux::State::Interface * nextBase = next;
          return nextBase->getUid() != uid;
        }

      private:

        Reducer::Interface<State> & _reducer;

    };

  }
}
