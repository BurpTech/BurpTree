#pragma once

namespace BurpRedux {

  template <class State>
  class Subscriber {

    public:

      virtual void setup(const State * initial) = 0;
      virtual void onPublish(const State * next) = 0;

  };

}
