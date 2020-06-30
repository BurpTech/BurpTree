#include "Instance.hpp"

namespace BurpRedux {
  namespace Reducer {

    Instance::Instance(const Id id, Deserializer & deserializer) :
      _id(id),
      _deserializer(deserializer)
    {}

    const Instance::State * Instance::deserialize(const JsonObject & object) {
      return _deserializer.deserialize(object);
    }

    const Instance::State * Instance::reduce(const Id id, const State * previous, const State * next) {
      if (id == _id) {
        return next;
      }
      return previous;
    }

  }
}
