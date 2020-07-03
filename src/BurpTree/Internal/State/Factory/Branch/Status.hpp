#pragma once

#include "../../../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {
        namespace Branch {

          class Status : public Internal::Status {

            public: 

              using Level = Status::Level;

              enum : Status::Code {
                ok,
                setupFailed,
                noDefault
              };

              const char * c_str() const override;

          };

        }
      }
    }
  }
}
