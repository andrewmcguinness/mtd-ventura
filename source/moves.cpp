#include "game.h"
#include "moves.h"

// return true if ok to go to next player
bool make_move(board& b, int player, move m) {
  return b.make_move(player, m.play, m.to);
}

std::ostream& operator << (std::ostream& out, move m) {
  if (m) {
    if (m.to)
      out << m.play << "->T" << m.to;
    else
      out << m.play << "->T*";
  } else out << "PASS";
  return out;
}
