#pragma once

#include "../src/BurpRedux/Reducer/Interface.hpp"

namespace BurpReduxTest {

  constexpr char dataField[] = "data";

  namespace A {

    constexpr char field[] = "a";

    namespace A {
      constexpr char field[] = "a";
    }
    namespace B {
      constexpr char field[] = "b";
    }
    namespace C {
      constexpr char field[] = "c";
    }

  }

  namespace B {

    constexpr char field[] = "b";

    namespace A {
      constexpr char field[] = "a";
    }
    namespace B {
      constexpr char field[] = "b";
    }
    namespace C {
      constexpr char field[] = "c";
    }

  }

  namespace ReducerId {
    enum : BurpRedux::Reducer::Interface::Id {
      aa,
      ab,
      ac,
      ba,
      bb,
      bc,
      count
    };
  }

}
