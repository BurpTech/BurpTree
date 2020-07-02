#pragma once

#include "../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      class Status : public Internal::Status {

        public: 

          using Level = Status::Level;

          enum : Status::Code {
            noError,
            dispatchDuringNotification,
            dispatchDuringDeserialize,
            dispatchDuringDispatch
          };

          const char * c_str() const override;

      };

    }
  }
}
