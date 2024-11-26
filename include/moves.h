#include <game.h>

#include <vector>
#include <algorithm>

struct move {
  tile play;
  short to;
};

int find_moves(const board& bd,
	       int player,
	       std::back_insert_iterator<std::vector<move>> out);
