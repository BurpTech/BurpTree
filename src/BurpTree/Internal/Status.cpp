#include "Status.hpp"

namespace BurpTree {
  namespace Internal {

    void Status::set(Level level, Code code) {
      _level = level;
      _code = code;
    }

    const Status::Level Status::getLevel() const {
      return _level;
    }

    const Status::Code Status::getCode() const {
      return _code;
    }

    #define C_STR_LABEL "BurpTree::Status"
    #define C_STR_DEFAULT BURP_TREE_C_STR_DEFAULT(C_STR_LABEL)
    const char * Status::c_str() const {
      switch (getCode()) {
        C_STR_DEFAULT;
      }
    }

  }
}
