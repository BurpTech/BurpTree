#pragma once

#include <functional>
#include <array>
#include "Action/Interface.hpp"
#include "Reducer/Interface.hpp"
#include "Creator/Interface.hpp"
#include "ReducerMapping/Interface.hpp"

namespace BurpRedux {

  template <
    class State,
    class Params,
    size_t size
  >
  class CombinedReducer : public Reducer::Interface<State, Params> {

    public:

      using Creator = Creator::Interface<State, State>;
      using ReducerMapping = ReducerMapping::Interface<State, Params>;
      using Map = std::array<ReducerMapping *, size>;

      CombinedReducer(Creator & creator, Map map) :
        _creator(creator),
        _map(map)
      {}

      void deserialize(const JsonObject & serialized, Params & params) override {
        for (auto mapping : _map) {
          mapping->deserialize(serialized, params);
        }
      }

      const State * init(const Params & params) override {
        State state;
        for (auto mapping : _map) {
          mapping->init(state, params);
        }
        return _creator.init(state);
      }

      const State * reduce(const State * previous, const Action::Interface & action) override {
        State state;
        auto modified = false;
        for (auto mapping : _map) {
          modified = mapping->reduce(previous, state, action) || modified;
        }
        if (modified) {
          // we don't know how to create a state, 
          // maybe the user wants to use a memory pool
          // or something
          return _creator.replace(previous, state);
        }
        return previous;
      }

    private:

      Creator & _creator;
      const Map _map;

  };

}
