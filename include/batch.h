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
batch_result() : points(0), games(0) {}
  int points;
  int games;
  std::string strategy;
};

class batch {
public:
  batch(std::vector<std::unique_ptr<strat>>&& strategies)
    : num_players(strategies.size()), strats(std::move(strategies)),
    starting_player(1), points(num_players), game_count(0), move_count(0)
  {
    for (int i = 0 ; i < num_players; ++i) points[i].strategy = strats[i]->desc();
  }
  std::vector<batch_result> results() const;
  int total_moves() const { return move_count; }
  void run_games(int num_games);
private:
  void run_game(int n);
  batch_result& score(int pl) { return points[pl-1]; }
  move get_move(const board& bd, int pl) { return (*strats[pl-1])(bd); }
  int num_players;
  std::vector<std::unique_ptr<strat>> strats;
  int starting_player;
  std::vector<batch_result> points;
  int game_count;
  int move_count;
};
