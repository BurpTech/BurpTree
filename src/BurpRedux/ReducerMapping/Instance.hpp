#pragma once

#include <functional>
#include "../Reducer/Interface.hpp"
#include "../State/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace ReducerMapping {

    template <class CombinedState, class State>
    using f_getState = const State * (*)(const CombinedState * state);

    template <class CombinedState, class State>
    using f_setState = void (*)(CombinedState & combinedState, const State * state);

    template <class CombinedParams, class Params>
    using f_getParams = Params & (*)(CombinedParams & params);

    template <class CombinedParams, class Params>
    using f_getConstParams = const Params & (*)(const CombinedParams & params);

    template <
      class CombinedState,
      class CombinedParams,
      class State,
      class Params,
      const char * serializedField,
      f_getState<CombinedState, State> getState,
      f_setState<CombinedState, State> setState,
      f_getParams<CombinedParams, Params> getParams,
      f_getConstParams<CombinedParams, Params> getConstParams
    >
    class Instance : public Interface<CombinedState, CombinedParams> {

      public:

        Instance(Reducer::Interface<State, Params> & reducer) :
          _reducer(reducer)
        {}

        void deserialize(const JsonObject & serialized, CombinedParams & params) override {
          _reducer.deserialize(serialized[serializedField].as<JsonObject>(), getParams(params));
        }

        void init(CombinedState & state, const CombinedParams & params) override {
           setState(state, _reducer.init(getConstParams(params)));
        }

        bool reduce(const CombinedState * state, CombinedState & temp, const Action::Interface & action) override {
          const State * current = getState(state);
          // we do this cast so that the compiler will
          // check that the user knows what they're doing.
          // States must implement the State base class and
          // expose a getUid method so that we can safely(?)
          // check if a state has (eally changed
          const BurpRedux::State::Interface * currentBase = current;
          unsigned long uid = currentBase->getUid();
          const State * next = _reducer.reduce(current, action);
          setState(temp, next);
          const BurpRedux::State::Interface * nextBase = next;
          return nextBase->getUid() != uid;
        }

      private:

        Reducer::Interface<State, Params> & _reducer;

    };

  }
}
