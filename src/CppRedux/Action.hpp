#pragma once

namespace CppRedux {

  template <class ActionType>
  class Action {

    public:

      const ActionType type;

      Action(const ActionType type, const void * payload = nullptr) :
        type(type),
        _payload(payload) {
      }

      template <class T>
      const T * payload() const {
        return static_cast<const T *>(_payload);
      }

    private:

      const void * _payload;

  };
}
