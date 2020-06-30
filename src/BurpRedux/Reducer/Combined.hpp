#pragma once

#include <array>
#include "../State/Combined.hpp"
#include "../State/Factory/Combined.hpp"
#include "../Selector/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <size_t length>
    class Combined : public Interface {

      public:

        using Reducer = Interface;
        using Selector = Selector::Interface;

        struct Entry {
          const char * field;
          Reducer * reducer;
          Selector * selector;
        };

        using Map = std::array<const Entry, length>;
        using State = BurpRedux::State::Combined<length>;
        using Factory = BurpRedux::State::Factory::Combined<length>;
        using StateInterface = BurpRedux::State::Interface;
        using Uid = StateInterface::Uid;
        using Uids = std::array<Uid, length>;
        using Index = typename State::Index;
        using States = typename State::States;
        using Fields = typename Factory::Fields;
        using Reducers = std::array<Reducer *, length>;
        using Selectors = std::array<Selector *, length>;
        using Id = Reducer::Id;

        Combined(const Map & map) :
          _lists(transposeMap(map)),
          _factory(_lists.fields)
        {
          for (size_t index = 0; index < length; index++) {
            _lists.selectors[index]->init(index);
          }
        }

        const StateInterface * deserialize(const JsonObject & object) override {
          States states;
          for (size_t index = 0; index < length; index++) {
            auto state = _lists.reducers[index]->deserialize(object[_lists.fields[index]].template as<JsonObject>());
            _uids[index] = state->getUid();
            states.set(index, state);
          }
          return _factory.create(states);
        }

        const StateInterface * reduce(const Id id, const StateInterface * previous, const StateInterface * next) override {
          States states;
          auto prevCombined = previous->as<State>();
          bool changed = false;
          for (size_t index = 0; index < length; index++) {
            auto nextState = _lists.reducers[index]->reduce(id, prevCombined->get(index), next);
            const Uid uid = nextState->getUid();
            if (uid != _uids[index]) {
              changed = true;
            }
            states.set(index, nextState);
            _uids[index] = uid;
          }
          if (changed) {
            return _factory.create(states);
          }
          return previous;
        }

      private:

        struct _Lists {
          Fields fields;
          Reducers reducers;
          Selectors selectors;
        };

        const _Lists _lists;
        Factory _factory;
        Uids _uids;

        static const _Lists transposeMap(const Map & map) {
          _Lists lists;
          for (Index index = 0; index < length; index++) {
            const Entry entry = map[index];
            lists.fields[index] = entry.field;
            lists.reducers[index] = entry.reducer;
            lists.selectors[index] = entry.selector;
          }
          return lists;
        }

    };

  }
}
