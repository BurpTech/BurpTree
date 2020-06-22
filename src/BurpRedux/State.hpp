#pragma once

namespace BurpRedux {

  template <class DerivedState>
  class State {

    public:

      virtual bool isNew(const DerivedState * current) const = 0;

  };

}
