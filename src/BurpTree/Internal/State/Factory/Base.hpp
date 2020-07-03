#pragma once

#include <functional>
#include <ArduinoJson.h>

namespace BurpTree {
  namespace Internal {
    namespace State {
      namespace Factory {

        template <class _State, class _Status>
        class Base {

          public:

            using State = _State;
            using Status = _Status;

            const State * getState() const {
              return _state;
            }

            const Status & getStatus() const {
              return _status;
            }

            virtual bool deserialize(const JsonObject & serialized) = 0;
            virtual bool createDefault() = 0;

          protected:

            using f_construct = std::function<const State * ()>;

            void * getAddress() const {
              return _address;
            }

            bool create(f_construct construct) {
              const State * state = construct();
              if (state) {
                _current++;
                _current %= _size;
                _address = &(_memory[_current]);
                if (_state) {
                  _state->~State();
                }
                _state = state;
                return true;
              }
              return false;
            };

            const State * error(typename Status::Code code) {
              _status.set(Status::Level::ERROR, code);
              return nullptr;
            }

            const State * warn(typename Status::Code code, const State * state) {
              _status.set(Status::Level::WARNING, code);
              return state;
            }

            const State * info(typename Status::Code code, const State * state) {
              _status.set(Status::Level::INFO, code);
              return state;
            }

            const State * ok(const State * state) {
              _status.set(Status::Level::INFO, Status::ok);
              return state;
            }

          private:

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
