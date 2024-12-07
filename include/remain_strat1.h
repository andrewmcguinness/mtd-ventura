#pragma once

#include <functional>
#include "game.h"
#include "moves.h"
#include "strat.h"

class remain_chain : public strat {
public:
  remain_chain(int pl, std::string description)
    : strat(pl), text(description) {}
  std::string desc() const {
    return text;
  }
  move operator () (const board& b) {
    std::vector<move> legal;
    find_moves(b, player, std::back_inserter(legal));
    if (legal.empty()) return pass;
    if (legal.size() == 1) return legal[0];

    auto hand = b.hand_for(player);
    short train = player;

    auto best_move{pass};
    chain_stats best{-1,0,0};
    auto copy = hand;
    auto my_chain_start = b.tracks[train].end;
    for (auto mv : legal) {
      chain_stats chain_length;
      auto& t = mv.play;
      std::iter_swap(copy.begin(), std::find(copy.begin(), copy.end(), t));
      if (mv.to == train) {
        chain_length = best_chain<longer_chain>(copy.begin() + 1, copy.end(),
                                                t.other(t.has(my_chain_start)));
      } else {
        chain_length = best_chain<longer_chain>(copy.begin() + 1, copy.end(),
                                                my_chain_start);
      }
      if ((best_move == pass) ||
          (chain_length.length > best.length) ||
          ((chain_length.length == best.length) && (t.score() > best_move.play.score()))) {
        best = chain_length;
        best_move = mv;
      }
    }
    return best_move;
  }
private:
  std::string text;
};
