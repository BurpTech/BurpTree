#include "State.hpp"

namespace BurpReduxTest {

  void deserialize(const JsonObject & serialized, Params & params) {
    params.data1 = serialized[data1Field];
    params.data2 = serialized[data2Field];
  }

}
