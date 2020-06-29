#pragma once

#include "../Status.hpp"

namespace BurpRedux {
  namespace Store {

    class Status : public BurpRedux::Status {

      public: 

        using Level = BurpRedux::Status::Level;

        enum Code : BurpRedux::Status::Code {
          noError,
          dispatchDuringNotification,
          dispatchDuringInit,
          dispatchDuringReduce
        };

        const char * c_str() const override {
          switch (getCode()) {
            case noError:
              return "BurpRedux::Store : no error";
            case dispatchDuringNotification:
              return "BurpRedux::Store : dispatch during notification";
            case dispatchDuringInit:
              return "BurpRedux::Store : dispatch during init";
            case dispatchDuringReduce:
              return "BurpRedux::Store : dispatch during reduce";
            default:
              return "BurpRedux::Store : unknown";
          }
        }

    };

  }
}
