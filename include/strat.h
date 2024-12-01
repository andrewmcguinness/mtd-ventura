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

template<typename valuator>
move best(std::vector<move>::iterator begin, std::vector<move>::iterator end,
	  valuator f) {
  scored_move choice{pass, 0};
  for (auto m = begin; m < end; ++m) {
    int score = f(*m);
    if (score > choice.score) choice = scored_move{*m, score};
  }
  return choice.m;
}

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

using tiles = std::vector<tile>;
int best_chain(tiles::iterator in, tiles::iterator in_end,
	       int start_val);

class long_home : public strat {
public:
  long_home(int pl) : strat(pl) {}
  move operator () (const board& b);
};
