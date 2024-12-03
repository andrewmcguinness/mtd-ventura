#include "game.h"
#include "strat.h"

bool better(chain_stats it, chain_stats than) {
  return it.length > than.length;
}

/* returns the length of the longest chain, with that chain
 * formed at the beginning of the vector
 */
chain_stats best_chain(tiles::iterator in, tiles::iterator in_end,
		       int start_val, chain_cmp cmp) {
  chain_stats best{0,0,0};
  std::vector<tile> saved_chain;
  for (auto t = in; t != in_end; ++t) {
    if (auto match = t->has(start_val)) {
      if (t != in)
	std::iter_swap(in, t); // now candidate is 1st,
      auto rest = best_chain(in+1, in_end, in->other(match), cmp) +
	chain_stats{1, in->dub(), in->score()};
      if (cmp(rest, best)) {
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

chain_stats longest_chain(tiles::iterator in, tiles::iterator in_end,
			  int start_val) {
  return best_chain(in, in_end, start_val, longer_chain{});
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
  auto chain_length = best_chain(hand.begin(), hand.end(),
				 b.tracks[player].end,
				 longer_chain{}).length;
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

move preserve_home::operator () (const board& b) {
  std::vector<move> legal;
  find_moves(b, player, std::back_inserter(legal));
  if (legal.empty()) return pass;
  if (legal.size() == 1) return legal[0];

  auto hand = b.hand_for(player);
  short train = player;
  auto chain_length = best_chain(hand.begin(), hand.end(),
				 b.tracks[player].end,
				 longer_chain{}).length;
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
    for (int i = 0; i < b.players; ++i) {
      // i=0 => track 0 (Mexican train)
      // i>1 => player i+1 (mod)
      auto rel = (prefer_common)?i:(b.players-i-1);
      short track = rel?(((rel + player) % b.players)+1):0;
      if ((track == 0) || (!b.tracks[track].train_on)) {
	auto value = b.tracks[track].end;
	auto pos = std::find_if(hand.begin() + chain_length, hand.end(),
				[value](auto t) { return t.has(value); });
	if (pos != hand.end())
	  return move{*pos, track};
      }
    }

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
