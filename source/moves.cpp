#include <vector>
#include <random>
#include <algorithm>
#include "game.h"
#include "moves.h"

int find_moves(const board& bd,
	       int player,
	       std::back_insert_iterator<std::vector<move>> out) {
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

