#pragma once

#include "State/Interface.hpp"
#include "Reducer/Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {

  class Store {

    public:

      class Status : public BurpRedux::Status {

        public: 

          using Level = BurpRedux::Status::Level;

          enum Code : BurpRedux::Status::Code {
            noError,
            dispatchDuringNotification,
            dispatchDuringDeserialize,
            dispatchDuringReduce
          };

          const char * c_str() const override;

      };

      using State = BurpRedux::State::Interface;
      using Reducer = BurpRedux::Reducer::Interface;
      using Id = Reducer::Id;

      Store(Reducer & reducer);
      void deserialize(const JsonObject & object);
      void loop();
      const Status & dispatch(const Id id, const State * next);
      const State * getState() const;

    private:

      Reducer & _reducer;
      bool _reducing;
      bool _notifying;
      bool _deserializing;
      const State * _next;
      Status _status;

  };

}
