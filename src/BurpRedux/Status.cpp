#include "Status.hpp"

namespace BurpRedux {

  const Status & Status::maxLevel(const Status & first, const Status & second) {
    return second.level > first.level ? second : first;
  }

  Status::Status() :
    level(Level::INFO),
    code(0)
  {}

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

}
