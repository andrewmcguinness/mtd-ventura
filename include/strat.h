#pragma once

#include "game.h"
#include "moves.h"

struct scored_move {
  move m;
  int score;
};

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
    scored_move choice{pass, 0};
    for (auto m : m1) {
      int score = 1000 + m.play.score() + 1000*(m.to == player);
      if (m.to)
	score -= 50*((m.to+b.players-player)%b.players);
      if (score > choice.score) choice = scored_move{m, score};
    }
    return choice.m;
  }
};
