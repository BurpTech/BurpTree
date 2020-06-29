#include "Instance.hpp"

namespace BurpRedux {
  namespace Reducer {

    Instance::Instance(const Id id) :
      _id(id)
    {}

    Instance::State * Instance::init(const StateList & list) {
      return list.get(_id);
    }

    Instance::State * Instance::reduce(const Id id, State * previous, State * next) {
      if (id == _id) {
        return next;
      }
      return previous;
    }

  }
}
