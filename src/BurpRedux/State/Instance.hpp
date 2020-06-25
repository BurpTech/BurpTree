#pragma once

#include "Interface.hpp"

namespace BurpRedux {
  namespace State {

    class Instance : public Interface {

      public:

        Instance(const unsigned long uid);
        const unsigned long getUid() const override;

      private:

        const unsigned long _uid;

    };

  }
}
