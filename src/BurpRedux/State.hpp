#pragma once

namespace BurpRedux {

  template <class DerivedState>
  class State {

    public:

      virtual unsigned long getUid() const = 0;

  };

}
