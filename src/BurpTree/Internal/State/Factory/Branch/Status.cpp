#include "Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {
        namespace Branch {

          #define C_STR_LABEL "BurpTree::State::Factory:Branch"
          #define C_STR_CASE(CODE) BURP_TREE_C_STR_CASE(C_STR_LABEL, CODE)
          #define C_STR_DEFAULT BURP_TREE_C_STR_DEFAULT(C_STR_LABEL)
          const char * Status::c_str() const {
            switch (getCode()) {
              C_STR_CASE(ok);
              C_STR_CASE(setupFailed);
              C_STR_CASE(noDefault);
              C_STR_DEFAULT;
            }
          }

        }
      }
    }
  }
}
