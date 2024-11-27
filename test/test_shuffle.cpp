#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "game.h"
#include <set>

TEST_CASE("shuffled") {
  depot it;
  std::set<tile> found;
  int count = 0;
  while (it.size() > 0) {
    ++count;
    auto next = it.take();
    CHECK(found.contains(next) == false);
    found.insert(next);
  }
  CHECK(count == 91);
  CHECK(found.contains(tile(12,0)) == true);
  CHECK(found.contains(tile(4,5)) == false);
  CHECK(found.contains(tile(6,5)) == true);
}

TEST_CASE("starting") {
  depot it;

  CHECK(it.take(tile(11,11)));
  CHECK(it.size() == 90);
  CHECK(!it.take(tile(11,11)));
}
