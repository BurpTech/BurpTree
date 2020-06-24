#pragma once

namespace BurpRedux {
  namespace Action {

    class Interface {

      public:

        virtual unsigned int getType() const = 0;

    };

  }
}
