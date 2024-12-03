#include "game.h"
#include "moves.h"

// return true if ok to go to next player
bool make_move(board& b, int player, move m) {
  return b.make_move(player, m.play, m.to);
}

std::optional<std::string> check_move(board& b, int player, move m) {
  if (m == pass) {
    std::vector<move> legal;
    if (find_moves(b, player, std::back_inserter(legal)))
      return "must move";
    else return {};
  }
  if (auto d = b.doubled())
    if (m.to != d->to) return "must play on double";
  if ((m.to != player) && (m.to != 0) && (b.tracks[m.to].train_on))
    return "track has train on";
  if (!b.tracks[m.to].can_add(m.play))
    return "tile does not match";
  return {};
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
