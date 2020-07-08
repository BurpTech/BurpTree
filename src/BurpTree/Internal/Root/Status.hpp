#pragma once

#include <BurpStatus.hpp>

namespace BurpTree {
  namespace Internal {
    namespace Root {

      class Status : public BurpStatus::Status {

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
