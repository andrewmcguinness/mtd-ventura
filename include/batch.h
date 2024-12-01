#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <memory>
#include <iostream>
#include "game.h"
#include "strat.h"

struct batch_result {
  int points;
  int games;
  std::string strategy;
};

class batch {
public:
  batch(std::vector<std::unique_ptr<strat>>&& strategies)
    : num_players(strategies.size()), strats(std::move(strategies)),
    starting_player(1), game_count(0)
  {
    for (int i = 0 ; i < num_players; ++i) points.push_back(0);
  }
  std::vector<batch_result> results() const;
  void run_games(int num_games);
private:
  void run_game(int n);
  int& score(int pl) { return points[pl-1]; }
  move get_move(const board& bd, int pl) { return (*strats[pl-1])(bd); }
  int num_players;
  std::vector<std::unique_ptr<strat>> strats;
  int starting_player;
  std::vector<int> points;
  int game_count;
};
