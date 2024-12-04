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
    return ((x.length == y.length) && (x.points > y.points)) ||
      (x.length > y.length);
  }
};

struct quicker_chain {
  bool operator() (chain_stats x, chain_stats y) {
    int x_turns = x.length + x.doubles;
    int y_turns = y.length + y.doubles;
    return ((x_turns == y_turns) && (x.points > y.points)) ||
      (x_turns > y_turns);
  }
};

struct fatter_chain {
  bool operator() (chain_stats x, chain_stats y) {
    return (x.points > y.points);
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

move move_on_other(const board& b, short player,
		   std::vector<tile>::iterator begin,
		   std::vector<tile>::iterator end,
		   int chain_length,
		   bool prefer_common);

// the idea of this strategy is to maintain as many tiles as
// possible to play on own track, while playing remaining tiles on
// any other available tracks whenever possible.
template<bool prefer_mx, typename cmp>
class preserve_home : public strat {
public:
  preserve_home(int pl, std::string description)
    : strat(pl), text(description) {}
  /*
    static std::unique_ptr<preserve_home> fat_chain(int pl, bool mode) {
    auto it = std::make_unique<preserve_home>(pl, mode);
    it->cmp = [](chain_stats x, chain_stats y) { return x.points > y.points; };
    it->text = std::string{"fat_chain_"} + (mode?"MX":"P");
    return it;
  }
  */
  std::string desc() const {
    return text;
  }
  move operator () (const board& b) {
    std::vector<move> legal;
    find_moves(b, player, std::back_inserter(legal));
    if (legal.empty()) return pass;
    if (legal.size() == 1) return legal[0];

    auto hand = b.hand_for(player);
    short train = player;
    auto chain_length = best_chain(hand.begin(), hand.end(),
				   b.tracks[player].end,
				   comparator).length;
    if (auto dd = b.doubled()) {
      if (dd->to == player) {
	auto pos = std::find_if(hand.begin(), hand.end(),
				[dd](auto t) { return t.has(dd->play.v1()); });
	return move{*pos, train};
      } else {
	auto pos = std::find_if(hand.rbegin(), hand.rend(),
				[dd](auto t) { return t.has(dd->play.v1()); });
	return move{*pos, dd->to};
      }
    } else {
      auto best = move_on_other(b, player,
				hand.begin(), hand.end(), chain_length,
				prefer_mx);
      if (best) return best;
    
      if (chain_length > 0) {
	if (hand[0].has(b.tracks[train].end))
	  return move{hand[0], train};
	else {
	  std::cout << "ERROR chain " << chain_length << " starts with "
		    << hand[0] << "\n";
	  return move{hand[0], train};
	}
      }
    }

    return best(legal.begin(), legal.end(),
		[&b](move m) {
		  return m.play.score();
		});
  }
private:
  cmp comparator;
  std::string text;
};
