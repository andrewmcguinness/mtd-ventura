#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "game.h"
#include "text.h"
#include "moves.h"
#include "strat.h"
#include "batch.h"

int hand_score(const std::vector<tile>& h) {
  return std::transform_reduce(h.begin(), h.end(), 0, std::plus<int>(),
			       [](auto t) { return t.score(); });
}

int main(int, char* []) {
  int iterations = 1000;
  std::vector<std::unique_ptr<strat>> strategies;
  //  strategies.push_back(std::make_unique<long_home>(1));
  strategies.push_back(preserve_home::fat_chain(1, true));
  strategies.push_back(std::make_unique<preserve_home>(2, true));
  strategies.push_back(preserve_home::fat_chain(3, true));
  strategies.push_back(std::make_unique<preserve_home>(4, true));

  batch runs(std::move(strategies));
  runs.run_games(iterations);
  int result_player = 0;
  std::cout << "\n\nAfter " << iterations << " full games:\n\n";
  for (auto p : runs.results()) {
    std::cout << "Player " << ++result_player <<
      "(" << p.strategy << ") : " << p.points << "\n";
  }
};
