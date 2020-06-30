#pragma once

#include "../src/BurpTree.hpp"

namespace BurpTreeTest {

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

  namespace NodeId {
    enum : BurpTree::Node::Id {
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
