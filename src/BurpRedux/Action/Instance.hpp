#pragma once

#include "Interface.hpp"

namespace BurpRedux {
  namespace Action {

    template <class Params, unsigned int type>
    class Instance : public Interface {

      public:

        Instance(const Params & params) :
          _params(params)
        {}

        unsigned int getType() const override {
          return type;
        }

        const Params & getParams() const {
          return _params;
        }

      private:

        const Params & _params;

    };

  }
}
