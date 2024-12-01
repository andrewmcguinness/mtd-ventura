#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "game.h"
#include "strat.h"
#include <vector>

TEST_CASE("no chain") {
  tiles it = { {3,2}, {4,3}, {5,4}, {5,3}, {4,2}, {5,2}, {6,2}, {6,3}, {6,5} };

  auto len = best_chain(it.begin(), it.end(), 9);
  CHECK(len == 0);
}

TEST_CASE("min chain") {
  tiles it = { {3,2}, {4,3}, {5,4}, {5,3}, {4,2}, {5,2}, {9,8}, {6,3}, {6,5} };

  auto len = best_chain(it.begin(), it.end(), 9);
  CHECK(len == 1);
  CHECK(it[0] == tile{9,8});
}

TEST_CASE("mid chain") {
  tiles it = { {3,2}, {4,3}, {8,7}, {5,3}, {4,2}, {5,2}, {9,8}, {7,1}, {11,7} };

  auto len = best_chain(it.begin(), it.end(), 9);
  CHECK(len == 3);
  CHECK(it[0] == tile{9,8});
  CHECK(it[1] == tile{8,7});
  CHECK(it[2].has(7));
}
