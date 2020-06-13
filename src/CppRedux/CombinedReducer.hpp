#pragma once

#include "Reducer.hpp"
#include "ReducerMap.hpp"

namespace CppRedux {

  template <class CombinedState, class Action>
  class CombinedReducer : public Reducer<CombinedState, Action> {

    public:

      CombinedReducer(const ReducerMap<CombinedState, Action> & map) :
        _map(map)
      {}

      const CombinedState * init(const CombinedState * previous, const CombinedState & initialState) const override {
        return _map.init(previous, initialState, [&](const CombinedState & state) {
          return Reducer<CombinedState, Action>::update(previous, state);
        });
      }

      const CombinedState * reduce(const CombinedState * previous, const Action & action) const override {
        return _map.reduce(previous, action, [&](const CombinedState & state) {
          return Reducer<CombinedState, Action>::update(previous, state);
        });
      }

    private:

      const ReducerMap<CombinedState, Action> & _map;

  };

}
