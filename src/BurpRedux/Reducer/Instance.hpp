#pragma once

#include <functional>
#include "../Action/Instance.hpp"
#include "../Creator/Interface.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class State, class Params, unsigned int type>
    class Instance : public Interface<State> {

      public:

        using Creator = Creator::Interface<State, Params>;

        Instance(Creator & creator) :
          _creator(creator)
        {}

        const State * reduce(const State * previous, const Action::Interface & action) override {
          switch (action.getType()) {
            case type: {
              using Action = Action::Instance<Params, type>;
              const Action & typedAction = dynamic_cast<const Action &>(action);
              return _creator.create(previous, typedAction.getParams());
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
