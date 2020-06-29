#pragma once

#include "Map.hpp"
#include "../../State/Combined/Instance.hpp"
#include "../../Creator/WithInitParams.hpp"
#include "../../State/List/Interface.hpp"

namespace BurpRedux {
  namespace Reducer {
    namespace Combined {

      template <size_t length>
      class Instance : public Reducer::Interface {

        public:

          using Entry = Map::Entry;
          using Map = Map::Instance<length>;
          using State = BurpRedux::State::Combined::Instance<length>;
          using Index = typename State::Index;
          using StateInterface = BurpRedux::State::Interface;
          using StateList = BurpRedux::State::List::Interface;
          using Creator = BurpRedux::Creator::WithInitParams<State, const Map &>;

          Instance(const Map map) :
            _map(map)
          {}

          StateInterface * init(const StateList & list) override {
            State * initial = _creator.init(_map);
            Index index = 0;
            for (auto mapping : _map) {
              mapping.selector->init(index);
              StateInterface * state = mapping.reducer->init(list);
              initial->set(index, state);
              index++;
            }
            return initial;
          }

          StateInterface * reduce(const Id id, StateInterface * previous, StateInterface * next) override {
            State * prevCombined = previous->as<State>();
            State * nextCombined = _creator.begin(prevCombined);
            Index index = 0;
            bool changed = false;
            for (auto mapping : _map) {
              auto prevState = prevCombined->get(index);
              auto nextState = mapping.reducer->reduce(id, prevState, next);
              if (nextState->getUid() != prevState->getUid()) {
                nextCombined->set(index, nextState);
                changed = true;
              }
              index++;
            }
            if (changed) {
              _creator.commit(prevCombined);
              return nextCombined;
            }
            return previous;
          }

        private:

          const Map _map;
          Creator _creator;

      };

    }
  }
}
