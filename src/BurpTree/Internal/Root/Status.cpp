#include "Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace Root {

      #define C_STR_LABEL "BurpTree::Root"
      #define C_STR_CASE(CODE) BURP_TREE_C_STR_CASE(C_STR_LABEL, CODE)
      #define C_STR_DEFAULT BURP_TREE_C_STR_DEFAULT(C_STR_LABEL)
      const char * Root::Status::c_str() const {
        switch (getCode()) {
          C_STR_CASE(noError);
          C_STR_CASE(dispatchDuringNotification);
          C_STR_CASE(dispatchDuringDeserialize);
          C_STR_CASE(dispatchDuringDispatch);
          C_STR_DEFAULT;
        }
      }

    }
  }
}
