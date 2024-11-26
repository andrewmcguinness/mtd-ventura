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

  std::vector<move> m1;
  find_moves(b, 1, std::back_inserter(m1));

  for (auto m : m1) {
    std::cout << m.play << "->" << m.to << " ";
  }
  std::cout << "\n";
};
