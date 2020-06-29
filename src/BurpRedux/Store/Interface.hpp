#pragma once

#include <ArduinoJson.h>
#include "../Publisher/Interface.hpp"
#include "../State/Interface.hpp"
#include "../State/List/Interface.hpp"
#include "../Reducer/Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {
  namespace Store {

    class Interface {

      public:

        using Id = BurpRedux::Reducer::Interface::Id;
        using StateInterface = BurpRedux::State::Interface;
        using StateList = BurpRedux::State::List::Interface;

        virtual void loop() = 0;
        virtual void init(const StateList & list) = 0;
        virtual const Status & dispatch(const Id id, StateInterface * next) = 0;

    };

  }
}
