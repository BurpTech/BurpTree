#pragma once

namespace BurpRedux {
  namespace State {

    class Interface {

      public:

        virtual const unsigned long getUid() const = 0;

    };

  }
}
