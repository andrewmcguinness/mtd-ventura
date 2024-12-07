#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <chrono>
#include "game.h"
#include "text.h"
#include "moves.h"
#include "strat.h"
#include "remain_strat1.h"
#include "remain_strat2.h"
#include "batch.h"

using std::chrono::milliseconds;

int hand_score(const std::vector<tile>& h) {
  return std::transform_reduce(h.begin(), h.end(), 0, std::plus<int>(),
			       [](auto t) { return t.score(); });
}

int main(int argc, char* argv[]) {
  int iterations = 1000;
  bool log = false;
  if ((argc > 1) && (std::string(argv[1]) == "once")) {
    iterations = 1;
    log = true;
  }
  std::vector<std::unique_ptr<strat>> strategies;
  strategies.push_back(std::make_unique<preserve_home<true, quicker_chain>>(1, "quick chains"));
  strategies.push_back(std::make_unique<remain_chain>(2, "long remaining chain"));
  strategies.push_back(std::make_unique<preserve_2<true, fatter_chain>>(3, "fat chains, highest first"));
  strategies.push_back(std::make_unique<remain_chain_d>(4, "long remaining chain, double aware"));

  batch runs(std::move(strategies), log);
  runs.run_games(iterations);
  int result_player = 0;
  std::cout << "\n\nAfter " << iterations << " full games:\n"
	    << runs.total_moves() << " moves\n";
  for (auto p : runs.results()) {
    std::cout << "Player " << ++result_player << "(" << p.strategy
	      << ") : "
	      << p.games << " wins, " << p.points << " points\n";
  }

  std::cout << "\n\nTimings\n";
  for (auto p : runs.results()) {
    std::cout << "Player " << ++result_player << "(" << p.strategy
	      << ") ave " << p.mean() << " t2 " << p.t2 << " sd "
	      << p.sd() << "\n";
  }
};
