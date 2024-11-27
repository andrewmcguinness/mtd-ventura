#include <iostream>
#include <random>
#include <vector>
#include "game.h"
#include "text.h"
#include "moves.h"

int main(int, char* []) {
  pool p;
  board b(4);
  
  deal(b, p);
  std::cout << b << "\n\n";

  int player = 1;
  while (b.winner() == 0) {
    std::vector<move> m1;
    find_moves(b, player, std::back_inserter(m1));

    for (auto m : m1) {
      std::cout << m << " ";
    }
    std::cout << "\n";

    if (m1.empty()) {
      if (!p.empty())
	b.hands[player-1].push_back(p.take());
      b.take_train(player);
    } else {
      make_move(b, player, m1.back());
    }
    
    std::cout << b << "\n\n";
    player = (player % b.players) + 1;
  }
};
