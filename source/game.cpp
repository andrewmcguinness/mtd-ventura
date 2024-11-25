#include <vector>
#include <random>
#include <algorithm>
#include <game.h>

pool::pool() {
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

bool pool::take(const tile& t) {
  auto f = std::find(tiles.begin(), tiles.end(), t);
  if (f == tiles.end()) return false;
  auto last = tiles.back();
  *f = last;
  tiles.pop_back();
  return true;
}

void deal(board& b, pool& p) {
  for (auto& h : b.hands)
    for (int i = 0; i < 15; ++i)
      h.push_back(p.take());
}