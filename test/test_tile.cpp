#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "game.h"
#include <set>

TEST_CASE("attrs") {
  tile t1(5,4);
  CHECK(t1.score() == 9);
  CHECK(!t1.has(3));
  CHECK(t1.has(4));
  CHECK(t1.has(5));
  CHECK(!t1.has(6));
  CHECK(t1.other(t1.has(4)) == 5);
  CHECK(t1.other(t1.has(5)) == 4);
  CHECK(!t1.dub());
}

TEST_CASE("double") {
  tile t1(5,5);
  CHECK(t1.score() == 10);
  CHECK(!t1.has(3));
  CHECK(!t1.has(4));
  CHECK(t1.has(5));
  CHECK(t1.other(t1.has(5)) == 5);
  CHECK(t1.dub());
}

TEST_CASE("zero") {
  tile t1(0, 0);
  CHECK(t1.score() == 30);
  CHECK(!t1.has(3));
  CHECK(!t1.has(4));
  CHECK(t1.has(0));
  CHECK(t1.other(t1.has(0)) == 0);
  CHECK(t1.dub());
}
