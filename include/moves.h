#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <string>
#include "game.h"
#include "text.h"

std::ostream& operator << (std::ostream& out, move m);

std::optional<std::string> check_move(board& b, int player, move m);

template <typename inserter>
int find_moves(const board& bd,
	       int player,
	       inserter out) {
  const std::vector<tile>& hand = bd.hand_for(player);
  int count = 0;
  auto dub = bd.doubled();
  if (dub) {
    auto need = dub->play.v1();
    for (auto t : hand) {
      if (t.has(need)) {
	*out = move(t, dub->to);
	++count;
      }
    }
  } else {
    for (size_t tn = 0; tn < bd.tracks.size(); ++tn) {
      const auto& tr = bd.tracks[tn];
      if ((!tr.train_on) || (tn == (size_t)player))
	for (auto t : hand) {
	  if (bd.tracks[tn].can_add(t)) {
	    *out = move(t, tn);
	    ++count;
	  }
	}
    }
  }
  return count;
}

// return true if ok to go to next player
bool make_move(board& b, int player, move m);
