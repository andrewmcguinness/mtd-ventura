#include <iostream>
#include <random>
#include "game.h"
#include "text.h"

int main(int, char* []) {
  pool p;
  board b(4);
  
  deal(b, p);
  std::cout << b << "\n\n";
};
