#pragma once

#include "Interface.hpp"

namespace BurpRedux {
  namespace State {

    class Instance : public Interface {

      public:

        Instance(const Uid uid);
        const unsigned int getUid() const override;
        const unsigned int getError() const override;
        void setError(Error error) override;

      private:

        const Uid _uid;
        Error _error;

    };

  }
}
