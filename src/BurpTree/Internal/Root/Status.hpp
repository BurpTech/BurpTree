#pragma once

#include "../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      class Status : public Internal::Status {

        public: 

          using Level = Status::Level;

          enum : Status::Code {
            ok,
            updateDuringNotification,
            updateDuringSetup,
            updateDuringUpdate
          };

          const char * c_str() const override;

      };

    }
  }
}
