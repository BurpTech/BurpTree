#pragma once

#include "Interface.hpp"
#include "../Creator/Instance.hpp"
#include "../StateList/Instance.hpp"
#include <array>

namespace BurpRedux {
  namespace Reducer {

    template <size_t length>
    class Combined : public Reducer::Interface {

      public:

        using Reducer = Reducer::Interface;

        struct Params {};
        struct Mapping {
          const char * field;
          const Reducer * reducer;
        };

        using Map = std::array<Mapping, length>;
        using CombinedState = BurpRedux::StateList::Instance<length>;
        using Creator = Creator::Instance<CombinedState, Params>;

        Combined(Map map) :
          _map(map)
        {}

        const State * init(const StateList & list) const override {
          _creator.init({});
        }

        void deserialize(State * current, const JsonObject & serialized) const override {
        }

        const State * reduce(const Id id, const State * previous, const State * next) const override {
        }

      private:

        const Map _map;
        Creator _creator;

    };

  }
}
