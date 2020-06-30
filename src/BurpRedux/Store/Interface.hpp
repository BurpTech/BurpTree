#pragma once

#include <ArduinoJson.h>
#include "../State/Interface.hpp"
#include "../Reducer/Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {
  namespace Store {

    template <class State>
    class Interface {

      public:

        using Id = BurpRedux::Reducer::Interface::Id;
        using StateInterface = BurpRedux::State::Interface;

        virtual void loop() = 0;
        virtual void deserialize(const JsonObject & object) = 0;
        virtual const Status & dispatch(const Id id, const StateInterface * next) = 0;
        virtual const State * getState() const = 0;

    };

  }
}
