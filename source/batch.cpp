#include "batch.h"

using std::chrono::nanoseconds;

std::vector<batch_result> batch::results() const {
  return points;
}

void batch::run_games(int num_games) {
  for (int n = 0; n < num_games; ++n) {
    std::cout << "Game " << n << "\n";
    ++game_count;
    run_game(n);
  }
}

void batch::run_game(int n) {
  std::vector<int> game_points(num_players);
  int player = starting_player;
  starting_player = 1 + (starting_player % num_players);

  for (int start = 12; start >= 0; --start) {
    std::cout << "Game : " << start << "|" << start << "\n";
    board b(num_players);
    b.set_start(start);

    auto seed = start + 17 * n;
    std::cout << "Seed " << seed << "\n";
    b.deal(pool(seed));
    std::cout << b << "\n\n";

    while ((b.winner() == 0) && !b.stalled()) {
      auto begin_tm = std::chrono::steady_clock::now();
      move m = get_move(b, player);
      auto calc_time = std::chrono::steady_clock::now() - begin_tm;
      score(player)(calc_time);
      if (calc_time > nanoseconds(1000000)) {
	std::cout << "Weird slow move " << calc_time << "\n"
		  << b << "\n" << m << "\n";
      }
      if (auto err = check_move(b, player, m)) {
	std::cout << "Player " << player << " ERROR\n" << b << "\n" << m <<
	  "\n" << *err << "\n";
	return;
      }
      if (m) {
	if (make_move(b, player, m))
	  player = b.next_player(player);
      } else {
	b.draw(player);
	player = b.next_player(player);
      }    
    }
    move_count += b.turns();
    std::cout << b << "\n\n";
    for (int pl = 1; pl <= num_players; ++pl) {
      int tot = 0;
      for (auto t : b.hand_for(pl))
	tot += t.score();
      score(pl).points += tot;
      game_points[pl-1] += tot;
      std::cout << "Player " << pl << " +" << tot << " = " << points[pl-1].points << "\n";
    }
  }
  auto game_winner = std::min_element(game_points.begin(), game_points.end())
    - game_points.begin();
  points[game_winner].games++;
}

