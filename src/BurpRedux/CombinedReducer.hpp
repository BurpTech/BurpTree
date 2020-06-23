#pragma once

#include <functional>
#include <array>
#include "Reducer/Interface.hpp"
#include "ReducerMapping/Interface.hpp"

namespace BurpRedux {

  template <class State, class Params, class Action, size_t size>
  class CombinedReducer : public Reducer::Interface<State, Action> {

    public:

      using f_create = std::function<const State * (const State * previous, const Params & params)>;
      using ReducerMapping = ReducerMapping::Interface<State, Params, Action>;
      using Map = std::array<ReducerMapping *, size>;

      CombinedReducer(f_create create, Map map) :
        _create(create),
        _map(map)
      {}

      const State * reduce(const State * previous, const Action & action) override {
        Params params;
        auto modified = false;
        for (auto mapping : _map) {
          modified = mapping->reduce(previous, params, action) || modified;
        }
        if (modified) {
          // we don't know how to create a state, 
          // maybe the user wants to use a memory pool
          // or something
          return _create(previous, params);
        }
        return previous;
      }

    private:

      f_create _create;
      const Map _map;

  };

}
