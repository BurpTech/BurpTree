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
  class CombinedReducer : public Reducer::Interface<State> {

    public:

      using Creator = Creator::Interface<State, Params>;
      using ReducerMapping = ReducerMapping::Interface<State, Params>;
      using Map = std::array<ReducerMapping *, size>;

      CombinedReducer(Creator & creator, Map map) :
        _creator(creator),
        _map(map)
      {}

      const State * deserialize(const JsonObject & serialized) override {
        Params params;
        for (auto mapping : _map) {
          mapping->deserialize(serialized, params);
        }
        return _creator.init(params);
      }

      const State * reduce(const State * previous, const Action::Interface & action) override {
        Params params;
        auto modified = false;
        for (auto mapping : _map) {
          modified = mapping->reduce(previous, params, action) || modified;
        }
        if (modified) {
          // we don't know how to create a state, 
          // maybe the user wants to use a memory pool
          // or something
          return _creator.replace(previous, params);
        }
        return previous;
      }

    private:

      Creator & _creator;
      const Map _map;

  };

}
