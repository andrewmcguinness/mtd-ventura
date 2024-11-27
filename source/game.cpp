#include <vector>
#include <random>
#include <algorithm>
#include <game.h>

move pass { {0, 0}, -1 };

pool::pool(long seed) {
  auto rand = std::minstd_rand0();
  if (seed) rand.seed(seed);
  for (int i = 0; i <= 12; ++i)
    for (int j = 0; j <= i; ++j)
      tiles.push_back(tile(i,j));
  std::shuffle(tiles.begin(), tiles.end(), std::minstd_rand0());
}

tile pool::take() {
  tile t = tiles.back();
  tiles.pop_back();
  return t;
}

bool pool::take(tile t) {
  auto f = std::find(tiles.begin(), tiles.end(), t);
  if (f == tiles.end()) return false;
  auto last = tiles.back();
  *f = last;
  tiles.pop_back();
  return true;
}

std::optional<move> board::doubled() const {
  for (size_t tn = 0; tn < tracks.size(); ++tn) {
    auto& tr = tracks[tn];
    if (tr.size() && tr.back().dub()) return move(tr.back(), tn);
  }
  return {};
}

void board::deal(pool& p) {
  for (auto& h : hands)
    for (int i = 0; i < 15; ++i)
      h.push_back(p.take());
}
