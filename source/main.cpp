#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "game.h"
#include "text.h"
#include "moves.h"
#include "strat.h"

int main(int, char* []) {
  pool tiles(1L);
  board b(4);
  
  b.deal(tiles);
  std::cout << b << "\n\n";

  std::vector<std::unique_ptr<strat>> strategies;
  strategies.push_back(nullptr);
  for (int p = 1; p <= b.players; ++p) {
    strategies.push_back(std::make_unique<dumbest>(p));
  }
  int player = 1;
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
};
