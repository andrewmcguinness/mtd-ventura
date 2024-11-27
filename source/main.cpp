#include <iostream>
#include <random>
#include <vector>
#include "game.h"
#include "text.h"
#include "moves.h"

int main(int, char* []) {
  pool p;
  board b(4);
  
  b.deal(p);
  std::cout << b << "\n\n";

  int player = 1;
  while (b.winner() == 0) {
    std::vector<move> m1;
    find_moves(b, player, std::back_inserter(m1));

    std::cout << "Player " << player << ": ";
    for (auto m : m1) {
      std::cout << m << " ";
    }
    std::cout << "\n";

    if (m1.empty()) {
      if (!p.empty())
	b.draw(player, p);
      b.take_train(player);
      player = b.next_player(player);
    } else {
      if (make_move(b, player, m1.back())) {
	player = b.next_player(player);
      }
    }
    
    std::cout << b << "\n\n";
  }
};
