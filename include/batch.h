#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <memory>
#include <iostream>
#include <chrono>
#include <cmath>
#include "game.h"
#include "strat.h"

using std::chrono::steady_clock;
using std::chrono::nanoseconds;

struct batch_result {
batch_result() : points(0), games(0), time(0L), t2(0L) {}
  int points;
  int games;
  int moves;
  steady_clock::duration time;
  long t2; // ns
  steady_clock::duration max;
  std::string strategy;
  steady_clock::duration mean() const { return time / moves; }
  void operator() (steady_clock::duration t) {
    time += t;
    moves += 1;
    if (t > max) max = t;
    long ns = std::chrono::duration_cast<nanoseconds>(t).count();
    t2 += (ns*ns);
  }
  double sd() const {
    double m = std::chrono::duration_cast<nanoseconds>(mean()).count();
    return (std::sqrt(((double(t2))/moves) - (m*m)));
  }
};

class batch {
public:
  batch(std::vector<std::unique_ptr<strat>>&& strategies, bool log)
    : num_players(strategies.size()), strats(std::move(strategies)),
      enable_log(log),
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
  bool enable_log;
  int starting_player;
  std::vector<batch_result> points;
  int game_count;
  int move_count;
};
