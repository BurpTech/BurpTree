#pragma once

#include <ArduinoJson.h>
#include "../Action/Interface.hpp"
#include "../Publisher/Interface.hpp"

namespace BurpRedux {
  namespace Store {

    enum class Error {
      noError,
      dispatchDuringNotificationWarning,
      dispatchDuringSetupWarning,
      dispatchDuringDeserializeError,
      dispatchDuringReduceError
    };

    template <class State>
    class Interface : public Publisher::Interface<State> {

      public:

        virtual void loop() = 0;
        virtual void deserialize(const JsonObject & serialized) = 0;
        virtual void setup(const State * initial) = 0;
        virtual Error dispatch(const Action::Interface & action) = 0;

    };

  }
}
