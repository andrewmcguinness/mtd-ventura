#include <iostream>
#include <random>
#include "game.h"

int main(int, char* []) {
  pool p;
  board b(4);
  
  deal(b, p);
  for (int i = 0; i < 31; ++i) {
    tile t = p.take();
    std::cout << "(" << t.v1() << "," << t.v2() << ") ";
  }
  std::cout << "\nremaining: " << p.size() << "\n";
};
