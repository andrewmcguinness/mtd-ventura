#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "game.h"
#include "strat.h"
#include <vector>
#include <chrono>

TEST_CASE("no chain") {
  tiles it = { {3,2}, {4,3}, {5,4}, {5,3}, {4,2}, {5,2}, {6,2}, {6,3}, {6,5} };

  auto len = best_chain<longer_chain>(it.begin(), it.end(), 9).length;
  CHECK(len == 0);
}

TEST_CASE("min chain") {
  tiles it = { {3,2}, {4,3}, {5,4}, {5,3}, {4,2}, {5,2}, {9,8}, {6,3}, {6,5} };

  auto len = best_chain<longer_chain>(it.begin(), it.end(), 9).length;
  CHECK(len == 1);
  CHECK(it[0] == tile{9,8});
}

TEST_CASE("mid chain") {
  tiles it = { {3,2}, {4,3}, {8,7}, {5,3}, {4,2}, {5,2}, {9,8}, {7,1}, {11,7} };

  auto len = best_chain<longer_chain>(it.begin(), it.end(), 9).length;
  CHECK(len == 3);
  CHECK(it[0] == tile{9,8});
  CHECK(it[1] == tile{8,7});
  CHECK(it[2].has(7));
}

TEST_CASE("error 1") {
  std::cout << "error 1\n";
  tiles it = { {11,3}, {12,11}, {4,0}, {6,0}, {5,3}, {11,11}, {12,8}, {11,5}, {12,5}, {11,10}, {6,1}, {1,0}, {8,6}, {10,2}, {7,0} };
  auto chain = best_chain<longer_chain>(it.begin(), it.end(), 12);
  CHECK(chain.length == 10);
  CHECK(it[0] == tile{12,11});
  CHECK(it[1] == tile{11,11});
  CHECK(it[2] == tile{11,3});
  CHECK(it[3] == tile{5,3});
  /*
  int link = 12;
  for (auto i = 0; i < chain.length; ++i) {
    auto t = it[i];
    auto match = t.has(link);
    auto old_link = link;
    CHECK((bool)match == true);
    link = t.other(match);
    if (t.dub()) {
      CHECK(old_link == link);
    } else {
      CHECK(old_link != link);
      CHECK(t.has(link));
    }
  }
  */
}

TEST_CASE("error 1b") {
  std::cout << "error 1b\n";
  tiles it = { {12,11}, {4,0}, {6,0}, {5,3}, {11,11}, {11,3}, {11,5}, {12,5}, {11,10}, {6,1}, {1,0}, {8,6}, {10,2}, {7,0} };
  auto chain = best_chain<longer_chain>(it.begin(), it.end(), 8);
  CHECK(chain.length == 4);
  CHECK(it[0] == tile{8,6});
  CHECK(it[1] == tile{6,1});
}

TEST_CASE("slow chain") {
  tiles it = {{9,3}, {7,0}, {3,2}, {7,1}, {1,1}, {7,5}, {11,0}, {6,0}, {1,0}, {5,5}, {6,3}, {2,2}, {12,0}, {5,1}, {5,2}, {11,7}, {6,5}, {12,6}, {12,7}};
  auto begin_tm = std::chrono::steady_clock::now();
  auto chain = best_chain<longer_chain>(it.begin(), it.end(), 1);
  auto calc_time = std::chrono::steady_clock::now() - begin_tm;
  CHECK(chain.length == 17);
  CHECK(calc_time < std::chrono::microseconds(10000));
}
