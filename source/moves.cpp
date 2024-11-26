#include "game.h"
#include "moves.h"

// return true if ok to go to next player
bool make_move(board& b, int player, move m) {
  b.hands[player-1].remove(m.play);
  b.tracks[m.to].add(m.play);
  if (m.to == player) b.tracks[m.to].train_on = true;
}


