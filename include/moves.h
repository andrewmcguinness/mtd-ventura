#include <iostream>
#include <vector>
#include <algorithm>
#include "game.h"
#include "text.h"

struct move {
  tile play;
  short to;
};

std::ostream& operator << (std::ostream& out, move m);

template <typename inserter>
int find_moves(const board& bd,
	       int player,
	       inserter out) {
  const std::vector<tile>& hand = bd.hands[player-1];
  int count = 0;
  for (size_t tn = 0; tn < bd.tracks.size(); ++tn) {
    const auto& tr = bd.tracks[tn];
    if ((!tr.train_on) || (tn == (size_t)player))
      for (const auto& t : hand) {
	if (bd.tracks[tn].can_add(t)) {
	  *out = move(t, tn);
	  ++count;
	}
      }
  }
  return count;
}

// return true if ok to go to next player
bool make_move(board& b, int player, move m);
