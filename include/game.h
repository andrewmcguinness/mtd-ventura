#pragma once

#include <vector>

struct tile {
  tile(int v1, int v2) : val((v1<<4) + v2) {}
  int v1() const { return ((unsigned)val) >> 4; }
  int v2() const { return val & 0x0f; }
  int v(int n) const { return (n==1)?v2():v1(); }
  bool operator == (const tile& other) const { return val == other.val; }
  bool operator < (const tile& other) const { return val < other.val; }
  bool operator > (const tile& other) const { return val > other.val; }
  unsigned char val;
};

struct pool {
  pool();
  int size() const { return tiles.size(); };
  tile take();
  std::vector<tile> tiles;
};

struct track {
  track(int s) : start(s), end(s), train_on(false) {}

  int start;
  int end;
  bool train_on;
  std::vector<tile> tiles;
};

struct board {
  board(int np) : players(np), start(12) {
    for (int i = 1; i <= players; ++i) {
      tracks.push_back(track(start));
      hands.push_back({});
    }
    tracks.push_back(track(start));
  }

  int players;
  int start;
  std::vector<track> tracks;
  std::vector<std::vector<tile>> hands;
};

void deal(board& b, pool& p);
