#include "Constants.hpp"
#include "State.hpp"

namespace BurpTreeTest {

  constexpr int defaultData = 0;

  State::State(const char * persistent, const int data) :
    persistent(persistent),
    data(data)
  {}

  State::State(const char * persistent) :
    State(persistent, defaultData)
  {}

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void Factory::setInitialPersistent(const char * persistent) {
    _persistent = persistent;
  }

  void Factory::createDefault() {
    create([&]() -> const State * {
        return new(getAddress()) State(_persistent);
    });
  }

  bool Factory::deserialize(const JsonObject & serialized) {
    return create([&]() -> const State * {
        const State * previous = getState();
        const char * persistent = previous ? previous->persistent : _persistent;
        if (!serialized.isNull()) {
          if (serialized[dataField].is<int>()) {
            return new(getAddress()) State(persistent, serialized[dataField].as<int>());
          }
          return error(Status::invalidData);
        }
        return error(Status::noObject);
    });
  }

  bool Factory::incrementData() {
    return create([&]() -> const State * {
        const State * previous = getState();
        return new(getAddress()) State(previous->persistent, previous->data + 1);
    });
  }

  bool Factory::setPersistent(const char * persistent) {
    return create([&]() -> const State * {
        const State * previous = getState();
        _persistent = persistent;
        return new(getAddress()) State(_persistent, previous->data);
    });
  }

  #define C_STR_LABEL "BurpTreeTest::State"
  #define C_STR_CASE(CODE) BURP_TREE_C_STR_CASE(C_STR_LABEL, CODE)
  #define C_STR_DEFAULT BURP_TREE_C_STR_DEFAULT(C_STR_LABEL)
  const char * Status::c_str() const {
    switch (getCode()) {
      C_STR_CASE(ok);
      C_STR_CASE(invalidData);
      C_STR_CASE(noObject);
      C_STR_DEFAULT;
    }
  }

}
