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
  strategies.push_back(std::make_unique<preserve_home<true, quicker_chain>>(1, "quick chains"));
  strategies.push_back(std::make_unique<preserve_home<true, fatter_chain>>(2, "long chains"));
  strategies.push_back(std::make_unique<preserve_2<true, fatter_chain>>(3, "fat chains, highest first"));
  strategies.push_back(std::make_unique<preserve_2<true, quicker_chain>>(4, "quick chains, highest first"));

  batch runs(std::move(strategies));
  runs.run_games(iterations);
  int result_player = 0;
  std::cout << "\n\nAfter " << iterations << " full games:\n"
	    << runs.total_moves() << " moves\n";
  for (auto p : runs.results()) {
    std::cout << "Player " << ++result_player << "(" << p.strategy << ") : "
	      << p.games << " wins, " << p.points << " points\n";
  }
};
