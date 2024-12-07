#pragma once

#include <functional>
#include "game.h"
#include "moves.h"
#include "strat.h"

// more double-aware version of remain_chain
class remain_chain_d : public strat {
public:
 remain_chain_d(int pl, std::string description)
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
    int best{-10};
    auto copy = hand;
    auto my_chain_start = b.tracks[train].end;
    for (auto mv : legal) {
      int chain_length;
      auto& t = mv.play;
      std::iter_swap(copy.begin(), std::find(copy.begin(), copy.end(), t));
      auto rest = copy.begin() + 1;
      auto my_chain_rest = my_chain_start;
      if (mv.to == train) my_chain_rest = t.other(t.has(my_chain_rest));
      if (t.dub()) {
        bool can_move_again = false;
        for (auto t2 = rest; t2 != copy.end() ; ++t2) {
          if (auto match = t2->has(t.v1())) {
            can_move_again = true;
            std::iter_swap(rest, t2);
            auto next_val = my_chain_rest;
            if (mv.to == train) next_val = t2->other(match);
            chain_length = best_chain<longer_chain>(rest + 1,
                                                    copy.end(), next_val).length + 1;
            if ((best_move == pass) ||
                (chain_length > best) ||
                ((chain_length == best) && (t.score() > best_move.play.score()))) {
              best = chain_length;
              best_move = mv;
            }
          }
        }
        if (!can_move_again) {
          chain_length = best_chain<longer_chain>(rest,
                                                  copy.end(), my_chain_rest).length - 1;
        }
        if ((best_move == pass) ||
            (chain_length > best) ||
            ((chain_length == best) && (t.score() > best_move.play.score()))) {
          best = chain_length;
          best_move = mv;
        }
      } else {
        chain_length = best_chain<longer_chain>(copy.begin() + 1, copy.end(),
                                                my_chain_rest).length;
      
        if ((best_move == pass) ||
            (chain_length > best) ||
            ((chain_length == best) && (t.score() > best_move.play.score()))) {
          best = chain_length;
          best_move = mv;
        }
      }
    }
    return best_move;
  }
private:
  std::string text;
};
