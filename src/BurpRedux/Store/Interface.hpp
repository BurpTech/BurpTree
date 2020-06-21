#pragma once

#include "../Publisher/Interface.hpp"

namespace BurpRedux {
  namespace Store {

    enum class Error {
      noError,
      dispatchDuringNotificationWarning,
      dispatchDuringReduceError
    };

    template <class State, class Action>
    class Interface : Publisher::Interface<State> {

      public:

        virtual void loop() = 0;
        virtual Error dispatch(const Action & action) = 0;

    };

  }
}
