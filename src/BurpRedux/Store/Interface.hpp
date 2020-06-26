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
      dispatchDuringInitError,
      dispatchDuringReduceError
    };

    template <class State, class Params>
    class Interface : public Publisher::Interface<State> {

      public:

        virtual void loop() = 0;
        virtual void deserialize(const JsonObject & serialized, Params & params) = 0;
        virtual void init(const Params & params) = 0;
        virtual void setup(const State * initial) = 0;
        virtual Error dispatch(const Action::Interface & action) = 0;

    };

  }
}
