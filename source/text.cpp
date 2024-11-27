#include "game.h"
#include "text.h"


ostream& operator << (ostream& o, tile t) {
  o << "[" << t.v1() << "|" << t.v2() << "]";
  return o;
}

ostream& operator << (ostream& o, const std::vector<tile>&hand) {
  int count = 0;
  for (tile t : hand) {
    if (count++) o << " ";
    o << t;
  }
  return o;
}

ostream& operator << (ostream& o, const track& tr) {
  o << (tr.train_on?" && ":"    ");
  int e = tr.start;
  for (tile t : tr.tiles) {
    auto next = t.other(t.has(e));
    o << "[" << e << "|" << next << "]";
    e = next;
  }
  o << "(" << tr.end << ")";
  return o;
}

ostream& operator << (ostream& o, const board& bd) {
  for (int p = 1 ; p <= bd.players; ++p)
    o << "P" << p << " " << bd.start << "-" << bd.tracks[p] << "\n";
  o << "  " << " " << bd.start << "-" << bd.tracks[0] << "\n";
  o << "\n";
  for (int p = 1 ; p <= bd.players; ++p)
    o << "Player " << p << ": " << bd.hand_for(p) << "\n";
  return o;
}

