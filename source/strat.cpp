#include "game.h"
#include "strat.h"

bool better(chain_stats it, chain_stats than) {
  return it.length > than.length;
}

move dumbest::operator () (const board& b) {
  std::vector<move> legal;
  find_moves(b, player, std::back_inserter(legal));
  return best(legal.begin(), legal.end(),
	      [&b, this](move m) {
		int score = 1000 + m.play.score() + 1000*(m.to == player);
		if (m.to)
		  score -= 50*((m.to+b.players-player)%b.players);
		return score;
	      });
}

move long_home::operator () (const board& b) {
  std::vector<move> legal;
  find_moves(b, player, std::back_inserter(legal));
  if (legal.empty()) return pass;
  if (legal.size() == 1) return legal[0];

  auto hand = b.hand_for(player);
  short train = player;
  auto chain_length = best_chain<longer_chain>(hand.begin(), hand.end(),
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
    if (chain_length > 0) {
      return move{hand[0], train};
    }
  }

  return best(legal.begin(), legal.end(),
	      [&b](move m) {
		return m.play.score();
	      });
}

chain_stats longest_chain_excluding(tiles::iterator in, tiles::iterator in_end,
                                    tile skip,
                                    int start_val) {
  chain_stats best{0,0,0};
  std::vector<tile> saved_chain;

  for (auto t = in; t != in_end; ++t) {
    if (*t != skip)
    if (auto match = t->has(start_val)) {
      if (t != in)
	std::iter_swap(in, t); // now candidate is 1st,
      auto rest = longest_chain_excluding(in+1, in_end, skip, in->other(match)) +
	chain_stats{1, in->dub(), in->score()};
      if (longer_chain{}(rest, best)) {
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
