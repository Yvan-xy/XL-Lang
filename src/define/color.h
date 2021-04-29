//
// Created by dyf on 2020/10/28.
//

#ifndef RJIT_COLOR_H
#define RJIT_COLOR_H

#include <ostream>

namespace Color {
  enum Code {
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_PINK = 35,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_PINK = 45,
    BG_DEFAULT = 49
  };

  class Modifier {
    Code code;
  public:
    Modifier(Code pCode) : code(pCode) {}

    friend std::ostream &
    operator<<(std::ostream &os, const Modifier &mod) {
      return os << "\033[" << mod.code << "m";
    }
  };
}

#endif //RJIT_COLOR_H
