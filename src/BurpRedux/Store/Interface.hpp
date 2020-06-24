#pragma once

#include "../Action/Interface.hpp"
#include "../Publisher/Interface.hpp"

namespace BurpRedux {
  namespace Store {

    enum class Error {
      noError,
      dispatchDuringNotificationWarning,
      dispatchDuringSetupWarning,
      dispatchDuringReduceError
    };

    template <class State>
    class Interface : Publisher::Interface<State> {

      public:

        virtual void loop() = 0;
        virtual Error dispatch(const Action::Interface & action) = 0;

    };

  }
}
