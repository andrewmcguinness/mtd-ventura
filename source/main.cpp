#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "game.h"
#include "text.h"
#include "moves.h"
#include "strat.h"

// todo identify algo for this
int hand_score(const std::vector<tile>& h) {
  int s = 0;
  for (auto t : h) s += t.score();
  return s;
}

int main(int, char* []) {
  int num_players = 4;
  std::vector<std::unique_ptr<strat>> strategies;
  strategies.push_back(nullptr);
  for (int p = 1; p <= num_players; ++p) {
    strategies.push_back(std::make_unique<dumbest>(p));
  }
  int player = 1;

  std::vector<int> points;
  for (int pl = 1; pl <= num_players; ++pl) points.push_back(0);

  for (int start = 12; start >= 0; --start) {
    std::cout << "Game : " << start << "|" << start << "\n";
    board b(num_players);
    b.set_start(start);

    auto seed = start;
    depot tiles(seed);
    tiles.take(tile{start,start});
  
    b.deal(tiles);
    std::cout << b << "\n\n";

    while (b.winner() == 0) {
      move m = (*strategies[player])(b);
      if (m) {
	if (make_move(b, player, m))
	  player = b.next_player(player);
      } else {
	if (!tiles.empty())
	  b.draw(player, tiles);
	b.take_train(player);
	player = b.next_player(player);
      }    
      std::cout << b << "\n\n";
    }
    for (int pl = 1; pl <= num_players; ++pl) {
      int tot = 0;
      for (auto t : b.hand_for(pl))
	tot += t.score();
      points[pl] += tot;
      std::cout << "Player " << pl << " +" << tot << " = " << points[pl] << "\n";
    }
  }
};
