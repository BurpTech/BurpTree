#pragma once

#include "Interface.hpp"

namespace BurpTree {
  namespace State {

    class Instance : public Interface {

      public:

        Instance(const Uid uid);
        const Uid getUid() const override;

      private:

        const Uid _uid;

    };

  }
}
