#pragma once

#include <functional>
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
  virtual std::string desc() const = 0;
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
  std::string desc() const { return "dumbest"; }
  move operator () (const board& b);
};

struct chain_stats {
  short length;
  short doubles;
  int points;
  chain_stats operator +(chain_stats y) const {
    chain_stats z;
    z.length = length + y.length;
    z.doubles = doubles + y.doubles;
    z.points = points + y.points;
    return z;
  }
};

using tiles = std::vector<tile>;
using chain_cmp = std::function<bool(chain_stats, chain_stats)>;
struct longer_chain {
  bool operator() (chain_stats x, chain_stats y) {
    return x.length > y.length;
  }
};

chain_stats best_chain(tiles::iterator in, tiles::iterator in_end,
		       int start_val, chain_cmp cmp);
chain_stats longest_chain(tiles::iterator in, tiles::iterator in_end,
			 int start_val);

class long_home : public strat {
public:
  long_home(int pl) : strat(pl) {}
  std::string desc() const { return "long_home"; }
  move operator () (const board& b);
};

class preserve_home : public strat {
public:
  preserve_home(int pl, bool mode)
    : strat(pl), prefer_common(mode), cmp(longer_chain{}),
      text(prefer_common?"preserve_home_MX":"preserve_home_P") {}
  static std::unique_ptr<preserve_home> fat_chain(int pl, bool mode) {
    auto it = std::make_unique<preserve_home>(pl, mode);
    it->cmp = [](chain_stats x, chain_stats y) { return x.points > y.points; };
    it->text = std::string{"fat_chain_"} + (mode?"MX":"P");
    return it;
  }
  std::string desc() const {
    return text;
  }
  move operator () (const board& b);
private:
  bool prefer_common;
  chain_cmp cmp;
  std::string text;
};
