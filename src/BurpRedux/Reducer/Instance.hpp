#pragma once

#include <functional>
#include "../Action/Instance.hpp"
#include "../Creator/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <
      class State,
      class Params,
      unsigned int type
    >
    class Instance : public Interface<State, Params> {

      public:

        using Creator = Creator::Interface<State, Params>;
        using f_deserialize = std::function<void(const JsonObject & serialized, Params & params)>;

        Instance(Creator & creator, f_deserialize deserialize) :
          _creator(creator),
          _deserialize(deserialize)
        {}

        void deserialize(const JsonObject & serialized, Params & params) override {
          _deserialize(serialized, params);
        }

        const State * init(const Params & params) override {
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
        f_deserialize _deserialize;

    };

  }
}
