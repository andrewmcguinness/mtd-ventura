#include "batch.h"

std::vector<batch_result> batch::results() const {
  std::vector<batch_result> out;
  for (int p = 0; p < num_players; ++p) {
    out.emplace_back(points[p], game_count, strats[p]->desc());
  }
  return out;
}

void batch::run_games(int num_games) {
  for (int n = 0; n < num_games; ++n) {
    std::cout << "Game " << n << "\n";
    ++game_count;
    run_game(n);
  }
}

void batch::run_game(int n) {
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
      move m = get_move(b, player);
      if (auto err = check_move(b, player, m)) {
	std::cout << "Player " << player << " ERROR\n" << b << "\n" << m <<
	  "\n" << *err << "\n";
	return;
      }
      //      std::cout << "Player " << player << " " << m << "\n";
      if (m) {
	if (make_move(b, player, m))
	  player = b.next_player(player);
      } else {
	b.draw(player);
	player = b.next_player(player);
      }    
    }
    std::cout << b << "\n\n";
    for (int pl = 1; pl <= num_players; ++pl) {
      int tot = 0;
      for (auto t : b.hand_for(pl))
	tot += t.score();
      score(pl) += tot;
      std::cout << "Player " << pl << " +" << tot << " = " << points[pl-1] << "\n";
    }
  }
}

