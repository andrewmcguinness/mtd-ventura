#pragma once

#include <vector>
#include <optional>

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

struct depot {
  depot(long seed = 0);
  int size() const { return tiles.size(); }
  int empty() const { return tiles.empty(); }
  tile take();
  bool take(tile t);
private:
  std::vector<tile> tiles;
};

struct track {
  track(int s) : start(s), end(s), train_on(false) {}
  int size() const { return tiles.size(); }
  tile back() const { return tiles.back(); }
  bool can_add(tile t) const { return t.has(end); }
  int add(tile t) {
    tiles.push_back(t);
    end = t.other(t.has(end));
    return end;
  }
  int start;
  int end;
  bool train_on;
  std::vector<tile> tiles;
};

struct move {
  tile play;
  short to;
  operator bool() const { return to >= 0; }
};

extern move pass;

struct board {
  board(int np) : players(np), start(12) {
    tracks.push_back(track(start));
    for (int i = 1; i <= players; ++i) {
      tracks.push_back(track(start));
      hands.push_back({});
      put_train(i);
    }
  }
  void deal(depot& p);
  void draw(int player, depot& p) { hand_for(player).push_back(p.take()); }
  void set_start(int d) { start = d; for (auto& t : tracks) t.start = t.end = start; }
  std::optional<move> doubled() const;
  bool can_use(int player, int track) {
    if (track == 0) return true;
    if (track == player) return true;
    return !tracks[track].train_on;
  }
  void take_train(int player) {
    tracks[player].train_on = false;
  }
  void put_train(int player) {
    tracks[player].train_on = true;
  }
  int winner() const {
    for (size_t pl = 1; pl <= hands.size(); ++pl)
      if (hand_for(pl).empty()) return pl;
    return 0;
  }
  int next_player(int last) const {
    return (last % players) + 1;
  }
  std::vector<tile>& hand_for(int player) {
    return hands[player - 1];
  }
  const std::vector<tile>& hand_for(int player) const {
    return hands[player - 1];
  }
  int players;
  int start;
  std::vector<track> tracks;
private:
  std::vector<std::vector<tile>> hands;
};

void deal(board& b, depot& p);
