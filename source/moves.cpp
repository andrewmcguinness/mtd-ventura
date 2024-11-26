#include "game.h"
#include "moves.h"

// return true if ok to go to next player
bool make_move(board& b, int player, move m) {
  std::ranges::remove(b.hands[player-1], m.play);
  //  b.hands[player-1].remove(m.play);
  b.tracks[m.to].add(m.play);
  if (m.to == player) b.tracks[m.to].train_on = true;

  return true; // fixme
}


std::ostream& operator << (std::ostream& out, move m) {
  if (m.to)
    out << m.play << "->T" << m.to;
  else
    out << m.play << "->T*";
  return out;
}
