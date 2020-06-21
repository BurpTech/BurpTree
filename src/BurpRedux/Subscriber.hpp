#pragma once

namespace BurpRedux {

  template <class State>
  class Subscriber {

    public:

      virtual void onPublish(const State * state) = 0;

  };

}
