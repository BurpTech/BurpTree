#pragma once

#include <array>
#include "../State/Combined.hpp"
#include "../State/Factory/Combined.hpp"
#include "../Publisher.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <size_t nodeCount, size_t subscriberCount>
    class Combined : public Interface {

      public:

        using Reducer = Interface;

        struct Entry {
          const char * field;
          Reducer * reducer;
        };

        using Map = std::array<const Entry, nodeCount>;
        using CombinedState = BurpRedux::State::Combined<nodeCount>;
        using Publisher = BurpRedux::Publisher<subscriberCount>;
        using Subscribers = typename Publisher::Subscribers;
        using Factory = BurpRedux::State::Factory::Combined<nodeCount>;
        using State = BurpRedux::State::Interface;
        using Uid = State::Uid;
        using Uids = std::array<Uid, nodeCount>;
        using Index = typename CombinedState::Index;
        using States = typename CombinedState::States;
        using Fields = typename Factory::Fields;
        using Reducers = std::array<Reducer *, nodeCount>;
        using Id = Reducer::Id;

        Combined(const Map & map, const Subscribers & subscribers) :
          _lists(transposeMap(map)),
          _factory(_lists.fields),
          _publisher(subscribers)
        {}

        const State * deserialize(const JsonObject & object) override {
          States states;
          for (Index index = 0; index < nodeCount; index++) {
            auto state = _lists.reducers[index]->deserialize(object[_lists.fields[index]].template as<JsonObject>());
            _uids[index] = state->getUid();
            states.set(index, state);
          }
          return _factory.create(states);
        }

        const State * reduce(const Id id, const State * next) override {
          States states;
          bool changed = false;
          for (Index index = 0; index < nodeCount; index++) {
            auto state = _lists.reducers[index]->reduce(id, next);
            const Uid uid = state->getUid();
            if (uid != _uids[index]) {
              changed = true;
            }
            states.set(index, state);
            _uids[index] = uid;
          }
          if (changed) {
            return _factory.create(states);
          }
          return _publisher.getState();
        }

        const State * getState() const override {
          return _publisher.getState();
        }

        void setup(const State * initial) override {
          const CombinedState * combined = initial->as<CombinedState>();
          for (Index index = 0; index < nodeCount; index++) {
            _lists.reducers[index]->setup(combined->get(index));
          }
          _publisher.setup(initial);
        }

        void onPublish(const State * next) override {
          const CombinedState * combined = next->as<CombinedState>();
          for (Index index = 0; index < nodeCount; index++) {
            _lists.reducers[index]->onPublish(combined->get(index));
          }
          _publisher.publish(next);
        }

      private:

        struct _Lists {
          Fields fields;
          Reducers reducers;
        };

        const _Lists _lists;
        Factory _factory;
        Uids _uids;
        Publisher _publisher;

        static const _Lists transposeMap(const Map & map) {
          _Lists lists;
          for (Index index = 0; index < nodeCount; index++) {
            const Entry entry = map[index];
            lists.fields[index] = entry.field;
            lists.reducers[index] = entry.reducer;
          }
          return lists;
        }

    };

  }
}
