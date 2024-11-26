#include <game.h>

#include <vector>
#include <algorithm>

struct move {
  tile play;
  short to;
};

ostream& operator << (ostream& out, move m) {
  if (m.to)
    out << m.play << "->T" << m.to;
  else
    out << m.play << "->T*";
  return out;
}

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
