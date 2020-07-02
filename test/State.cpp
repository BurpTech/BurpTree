#include "Constants.hpp"
#include "State.hpp"

namespace BurpTreeTest {

  constexpr int defaultData = 0;

  State::State(const Uid uid, const char * persistent, const int data) :
    BurpTree::State(uid),
    persistent(persistent),
    data(data)
  {}

  State::State(const Uid uid, const char * persistent) :
    State(uid, persistent, defaultData)
  {}

  void State::serialize(const JsonObject & serialized) const {
    serialized[dataField] = data;
  }

  void Factory::setInitialPersistent(const char * persistent) {
    _persistent = persistent;
  }

  const BurpTree::State * Factory::deserialize(const JsonObject & serialized) {
    return create([&]() -> const State * {
        const State * previous = getState();
        const char * persistent = previous ? previous->persistent : _persistent;
        if (!serialized.isNull()) {
          if (serialized[dataField].is<int>()) {
            return new(getAddress()) State(getUid(), persistent, serialized[dataField].as<int>());
          }
          return fail(Status::invalidData);
        }
        return fail(Status::noObject);
    });
  }

  const BurpTree::State * Factory::incrementData() {
    return create([&]() -> const State * {
        const State * previous = getState();
        return new(getAddress()) State(getUid(), previous->persistent, previous->data + 1);
    });
  }

  const BurpTree::State * Factory::setPersistent(const char * persistent) {
    return create([&]() -> const State * {
        const State * previous = getState();
        _persistent = persistent;
        return new(getAddress()) State(getUid(), _persistent, previous->data);
    });
  }

  const State * Factory::_default() {
    return new(getAddress()) State(getUid(), _persistent);
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
