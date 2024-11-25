#pragma once

#include <vector>

struct tile_pos {
  bool m1;
  bool m2;
  operator bool() const { return m1 || m2; }
};

struct tile {
  tile(int v1, int v2) : val((v1<<4) + v2) {}
  int v1() const { return ((unsigned)val) >> 4; }
  int v2() const { return val & 0x0f; }
  int v(int n) const { return (n==1)?v2():v1(); }
  bool dub() const { return v1() == v2(); }
  int score() const { return v1() + v2() + (val?0:30); }
  tile_pos has(int n) const { return { v1() == n, v2() == n }; } 
  int other(const tile_pos& tp) const { return tp.m1?(v2()):v1(); }
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
  bool add(tile t);

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
