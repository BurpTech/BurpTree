#include "Instance.hpp"

namespace BurpRedux {
  namespace State {

    Instance::Instance(const Uid uid) :
      _uid(uid),
      _error(noError)
    {}

    const Instance::Uid Instance::getUid() const {
      return _uid;
    }

    const Instance::Error Instance::getError() const {
      return _error;
    }

    void Instance::setError(Error error) {
      _error = error;
    }

  }
}
