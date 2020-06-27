#include "Instance.hpp"

namespace BurpRedux {
  namespace Reducer {

    Instance::Instance(const Id id) :
      _id(id)
    {}

    const Instance::State * Instance::init(const StateList & list) const {
      return list.get(_id);
    }

    void Instance::deserialize(State * current, const JsonObject & serialized) const {
      current->deserialize(serialized);
    }

    const Instance::State * Instance::reduce(const Id id, const State * previous, const State * next) const {
      if (id == _id) {
        return next;
      }
      return previous;
    }

  }
}
