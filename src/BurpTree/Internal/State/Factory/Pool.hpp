#pragma once

#include <functional>
#include "../Base.hpp"
#include "../../Status.hpp"

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <class State, class Status>
        class Pool {

          public:

            const Status & getStatus() const {
              return _status;
            }

          protected:

            using Uid = Base::Uid;
            using f_construct = std::function<const State * ()>;

            const Uid getUid() const {
              return _uid;
            }

            void * getAddress() const {
              return _address;
            }

            const State * getState() const {
              return _state;
            }

            const State * create(f_construct construct) {
              _status.set(Status::Level::INFO, Status::noError);
              const State * state = construct();
              if (state) {
                _uid++;
                _current++;
                _current %= _size;
                _address = &(_memory[_current]);
                _setState(state);
              }
              return state;
            };

            const State * fail(const typename Status::Code code) {
              if (_state) {
                _status.set(Status::Level::ERROR, code);
                return nullptr;
              }
              _status.set(Status::Level::WARNING, code);
              return _default();
            }

          private:

            virtual const State * _default();

            void _setState(const State * state) {
              if (_state) {
                _state->~State();
              }
              _state = state;
            }

            Uid _uid;
            static constexpr size_t _size = 2;
            char _memory[_size][sizeof(State)];
            unsigned char _current = 0;
            void * _address = &(_memory[_current]);
            const State * _state = nullptr;
            Status _status;

        };

      }
    }
  }
}
