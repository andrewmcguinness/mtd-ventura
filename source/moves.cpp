#include "game.h"
#include "moves.h"

// return true if ok to go to next player
bool make_move(board& b, int player, move m) {
  auto& hand = b.hands[player-1];
  auto dead = std::ranges::remove(hand, m.play);
  hand.erase(dead.begin());
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
