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
  std::vector<move> m1;
  find_moves(b, player, std::back_inserter(m1));

  for (auto m : m1) {
    std::cout << m << " ";
  }
  std::cout << "\n";

  make_move(b, player, m.back());

  std::cout << b << "\n\n";
};
