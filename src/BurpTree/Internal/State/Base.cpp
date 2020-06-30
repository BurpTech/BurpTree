#include "Base.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {

      Base::Base(const Uid uid) :
        _uid(uid)
      {}

      const Base::Uid Base::getUid() const {
        return _uid;
      }

    }
  }
}
