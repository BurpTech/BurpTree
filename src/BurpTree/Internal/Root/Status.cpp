#include "Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      #define C_STR_LABEL "BurpTree::Root"
      #define C_STR_CASE(CODE) BURP_STATUS_C_STR_CASE(C_STR_LABEL, CODE)
      #define C_STR_DEFAULT BURP_STATUS_C_STR_DEFAULT(C_STR_LABEL)
      const char * Status::c_str() const {
        switch (getCode()) {
          C_STR_CASE(ok);
          C_STR_CASE(updateDuringNotification);
          C_STR_CASE(updateDuringSetup);
          C_STR_CASE(updateDuringUpdate);
          C_STR_DEFAULT;
        }
      }

    }
  }
}
