#include "game.h"
#include "strat.h"

/* returns the length of the longest chain, with that chain
 * formed at the beginning of the vector
 */
int best_chain(tiles::iterator in, tiles::iterator in_end,
	       int start_val) {
  int best = 0;
  for (auto t = in; t != in_end; ++t) {
    if (auto match = t->has(start_val)) {
      if (t != in)
	std::swap(*in, *t); // now candidate is 1st
      auto rest = best_chain(in+1, in_end, in->other(match)) + 1;
      if (rest <= best)
	std::swap(*in, *t); // swap back because it didn't improve
      else best = rest;
    }
  }
  return best;
}

move long_home::operator () (const board& b) {
  std::vector<move> legal;
  find_moves(b, player, std::back_inserter(legal));
  if (legal.empty()) return pass;
  if (legal.size() == 1) return legal[0];

  auto hand = b.hand_for(player);
  short train = player;
  auto chain_length = best_chain(hand.begin(), hand.end(),
				 b.tracks[player].end);
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
    if (chain_length > 0) {
      return move{hand[0], train};
    }
  }

  return best(legal.begin(), legal.end(),
	      [&b](move m) {
		return m.play.score();
	      });
}
