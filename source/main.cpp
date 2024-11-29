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
  int num_players = 4;
  std::vector<std::unique_ptr<strat>> strategies;
  for (int p = 1; p <= num_players; ++p) {
    strategies.push_back(std::make_unique<dumbest>(p));
  }

  batch runs(std::move(strategies));
  runs.run_games(1000);
  int result_player = 0;
  for (auto p : runs.results())
    std::cout << "Player " << (++result_player) << " : " << p << "\n";
};
