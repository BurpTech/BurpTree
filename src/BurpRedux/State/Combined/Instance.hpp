#pragma once

#include "../Instance.hpp"
#include "../List/Instance.hpp"
#include "../../Reducer/Combined/Map.hpp"

namespace BurpRedux {
  namespace State {
    namespace Combined {

      class Status : public BurpRedux::Status {

        public: 

          using Level = BurpRedux::Status::Level;

          enum Code : BurpRedux::Status::Code {
            noError
          };

          const char * c_str() const override {
            switch (getCode()) {
              case noError:
                return "BurpRedux::State::Combined : no error";
              default:
                return "BurpRedux::State::Combined : unknown";
            }
          }

      };

      template <size_t length>
      class Instance : public State::Instance, public List::Instance<length> {

        public:

          using Map = Reducer::Combined::Map::Instance<length>;
          using StateList = BurpRedux::State::List::Instance<length>;
          using Index = typename StateList::Index;
          using State = BurpRedux::State::Instance;
          using StateInterface = BurpRedux::State::Interface;

          Instance(const Map & map, const Uid uid) :
            State(uid),
            _map(map)
          {}

          Instance(const Instance * previous, const Uid uid) :
            State(uid),
            StateList(previous->_states),
            _map(previous->_map)
          {}

          const Uid getUid() const override {
            return State::getUid();
          }

          const Status & getStatus() const override {
            return _status;
          }

          void serialize(const JsonObject & serialized) const override {
            size_t index = 0;
            for (auto mapping : _map) {
              get(index)->serialize(serialized[mapping.field].template to<JsonObject>());
              index++;
            }
          }

          void deserialize(const JsonObject & serialized) override {
            size_t index = 0;
            for (auto mapping : _map) {
              get(index)->deserialize(serialized[mapping.field].template as<JsonObject>());
              index++;
            }
          }

          StateInterface * get(const Index index) const override {
            return StateList::get(index);
          }

          void set(const Index index, StateInterface * state) override {
            StateList::set(index, state);
          }

        private:

          const Map & _map;
          const Status _status;

      };

    }
  }
}
