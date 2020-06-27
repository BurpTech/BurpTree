#pragma once

#include <functional>
#include "State/Interface.hpp"
#include "Store/Interface.hpp"
#include "Creator/Interface1.hpp"

namespace BurpRedux {

  template <class State>
  class Dispatcher {

    public:

      using Base = BurpRedux::State::Interface;
      using Store = BurpRedux::Store::Interface;
      using Creator = BurpRedux::Creator::Interface1<State>;
      using f_assign = std::function<void(const State * previous, State * next)>;

      Dispatcher(Store & store, Creator & creator, const unsigned int id) :
        _store(store),
        _creator(creator),
        _id(id)
      {}

      const unsigned int dispatch(const State * previous, f_assign assign) {
        State * next = _creator.begin(previous);
        assign(previous, next);
        Base * base = next;
        const Base::Error error = base->getError();
        if (Base::noError == error) {
          _creator.commit(previous);
          _store.dispatch(_id, base);
        }
        return error;
      }

    private:

      Store _store;
      Creator _creator;
      const unsigned int _id;

  };

}
