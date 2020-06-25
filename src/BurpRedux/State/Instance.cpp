#include "Instance.hpp"

namespace BurpRedux {
  namespace State {

    Instance::Instance(const unsigned long uid) :
      _uid(uid)
    {}

    const unsigned long Instance::getUid() const {
      return _uid;
    }

  }
}
