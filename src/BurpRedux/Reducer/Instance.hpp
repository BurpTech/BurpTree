#pragma once

#include <functional>
#include "../Action/Instance.hpp"
#include "Interface.hpp"

namespace BurpRedux {
  namespace Reducer {

    template <class State, class Params, unsigned int type>
    class Instance : public Interface<State> {

      public:

        using f_create = std::function<const State * (const State * previous, const Params & params)>;

        Instance(f_create create) :
          _create(create)
        {}

        const State * reduce(const State * previous, const Action::Interface & action) override {
          switch (action.getType()) {
            case type: {
              using Action = Action::Instance<Params, type>;
              const Action & typedAction = dynamic_cast<const Action &>(action);
              return _create(previous, typedAction.getParams());
            }
            default:
              return previous;
          }
        }

      private:

        f_create _create;

    };

  }
}
