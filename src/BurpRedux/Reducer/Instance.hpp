#pragma once

#include <functional>
#include "../Action/Instance.hpp"
#include "../Creator/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class Params>
    using f_deserialize = void (*)(const JsonObject & serialized, Params & params);

    template <
      class State,
      class Params,
      f_deserialize<Params> deserializeFunction,
      unsigned int type
    >
    class Instance : public Interface<State> {

      public:

        using Creator = Creator::Interface<State, Params>;

        Instance(Creator & creator) :
          _creator(creator)
        {}

        const State * deserialize(const JsonObject & serialized) override {
          Params params;
          deserializeFunction(serialized, params);
          return _creator.init(params);
        }

        const State * reduce(const State * previous, const Action::Interface & action) override {
          switch (action.getType()) {
            case type: {
              using Action = Action::Instance<Params, type>;
              const Action & typedAction = static_cast<const Action &>(action);
              return _creator.replace(previous, typedAction.getParams());
            }
            default:
              return previous;
          }
        }

      private:

        Creator & _creator;

    };

  }
}
