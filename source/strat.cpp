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

move move_on_other(const board& b, short player,
		   std::vector<tile>::iterator begin,
		   std::vector<tile>::iterator end,
		   int chain_length,
		   bool prefer_common) {
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
