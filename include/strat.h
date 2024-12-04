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

/* returns the length of the longest chain, with that chain
 * formed at the beginning of the vector
 */
template<typename comparator>
chain_stats best_chain(tiles::iterator in, tiles::iterator in_end,
		       int start_val) {
  chain_stats best{0,0,0};
  std::vector<tile> saved_chain;

  for (auto t = in; t != in_end; ++t) {
    if (auto match = t->has(start_val)) {
      if (t != in)
	std::iter_swap(in, t); // now candidate is 1st,
      auto rest = best_chain<comparator>(in+1, in_end, in->other(match)) +
	chain_stats{1, in->dub(), in->score()};
      if (comparator{}(rest, best)) {
	best = rest;
	saved_chain.assign(in, in_end);
      } else {
	if (best.length > 0)
	  std::copy(saved_chain.begin(), saved_chain.end(), in);
      }
      // at this point *in is right, and 
      // and either it is right, or saved_chain is right and dirty is true
    }
  }
  return best;
}

class long_home : public strat {
public:
  long_home(int pl) : strat(pl) {}
  std::string desc() const { return "long_home"; }
  move operator () (const board& b);
};

template<bool prefer_common>
move move_on_other_simple(const board& b, short player,
			  std::vector<tile>::iterator begin,
			  std::vector<tile>::iterator end,
			  int chain_length)  {
  for (int i = 0; i < b.players; ++i) {
    // i=0 => track 0 (Mexican train)
    // i>1 => player i+1 (mod)
    auto rel = (prefer_common)?i:(b.players-i-1);
    short track = rel?(((rel + player) % b.players)+1):0;
    if ((track == 0) || (!b.tracks[track].train_on)) {
      auto value = b.tracks[track].end;
      auto pos = std::find_if(begin + chain_length, end,
			      [value](auto t) { return t.has(value); });
      if (pos != end)
	return move{*pos, track};
    }
  }
  return pass;
}

template<bool prefer_common>
move move_other_highest(const board& b, short player,
			std::vector<tile>::iterator begin,
			std::vector<tile>::iterator end,
			int chain_length)  {
  for (int i = 0; i < b.players; ++i) {
    // i=0 => track 0 (Mexican train)
    // i>1 => player i+1 (mod)
    auto rel = (prefer_common)?i:(b.players-i-1);
    short track = rel?(((rel + player) % b.players)+1):0;
    if ((track == 0) || (!b.tracks[track].train_on)) {
      auto value = b.tracks[track].end;
      auto non_chain = begin + chain_length;
      auto pos = std::partition(non_chain, end,
				[value](auto t) { return t.has(value); });
      if (pos != non_chain) {
	auto best = std::max_element(non_chain, pos,
				     [](tile x, tile y) { return x.score() < y.score(); });
	return move{*best, track};
      }
    }
  }
  return pass;
}

// the idea of this strategy is to maintain as many tiles as
// possible to play on own track, while playing remaining tiles on
// any other available tracks whenever possible.
template<bool prefer_mx, typename cmp>
class preserve_home : public strat {
public:
  preserve_home(int pl, std::string description)
    : strat(pl), text(description) {}
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
    auto chain_length = best_chain<cmp>(hand.begin(), hand.end(),
					b.tracks[player].end).length;
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
      auto best = move_on_other_simple<prefer_mx>(b, player,
						  hand.begin(), hand.end(),
						  chain_length);
      if (best) return best;
    
      if (chain_length > 0) {
	  return move{hand[0], train};
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

template<bool prefer_mx, typename cmp>
class preserve_2 : public strat {
public:
  preserve_2(int pl, std::string description)
    : strat(pl), text(description) {}
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
    auto chain_length = best_chain<cmp>(hand.begin(), hand.end(),
					b.tracks[player].end).length;
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
      auto best = move_other_highest<prefer_mx>(b, player,
						hand.begin(), hand.end(),
						chain_length);
      if (best) return best;
    
      if (chain_length > 0) {
	  return move{hand[0], train};
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
