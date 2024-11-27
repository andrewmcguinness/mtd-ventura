#pragma once

#include "game.h"

class strat {
public:
  strat(int p) : player(p) {}
  virtual move operator ()(const board& b) = 0;
  virtual ~strat() {}
protected:
  int player;
};

class dumbest : public strat {
public:
  dumbest(int pl) : strat(pl) {}
  move operator () (const board& b) {
    std::vector<move> m1;
    find_moves(b, player, std::back_inserter(m1));
    if (m1.empty()) return pass;
    return m1.back();
  }
};
