#pragma once

#include <functional>
#include "State/Interface.hpp"
#include "Store/Interface.hpp"
#include "Creator/Interface.hpp"
#include "Status.hpp"

namespace BurpRedux {

  template <class State>
  class Dispatcher {

    public:

      using Base = BurpRedux::State::Interface;
      using Store = BurpRedux::Store::Interface;
      using Creator = BurpRedux::Creator::Interface<State>;
      using f_assign = std::function<void(State * next, const State * previous)>;

      Dispatcher(Store & store, Creator & creator, const unsigned int id) :
        _store(store),
        _creator(creator),
        _id(id)
      {}

      const Status & dispatch(const State * previous, f_assign assign) {
        State * next = _creator.begin(previous);
        assign(next, previous);
        Base * base = next;
        const Status & status = base->getStatus();
        if (BurpRedux::Status::Level::ERROR == status.level) {
          return status;
        }
        _creator.commit(previous);
        return Status::maxLevel(status, _store.dispatch(_id, base));
      }

    private:

      Store & _store;
      Creator & _creator;
      const unsigned int _id;

  };

}
