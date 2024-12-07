#pragma once

#include <functional>
#include "game.h"
#include "moves.h"
#include "strat.h"

// more double-aware version of remain_chain
class remain_chain_d : public strat {
  struct accum {
    accum() : best_score{}, best_move{pass} {}
    bool add_x(int len, int score, move mv) {
      if ((best_move == pass) ||
          (len > best_length) ||
          ((len == best_length) && (score > best_score))) {
        best_length = len;
        best_score = score;
        best_move = mv;
        return true;
      }
      return false;
    }
    bool add(int len, tile t2, move mv) {
      return add_x(len + 1, mv.play.score() + t2.score(), mv);
    }
    bool add(int len, move mv) {
      return add_x(len, mv.play.score(), mv);
    }
    int best_length;
    int best_score;
    move best_move;
  };
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

    accum best;
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
                                                    copy.end(), next_val).length;
            best.add(chain_length, t, mv);
          }
        }
        if (!can_move_again) {
          // apply a penalty because will have to draw
          chain_length = best_chain<longer_chain>(rest,
                                                  copy.end(), my_chain_rest).length - 1;
          best.add(chain_length, mv);
        }
      } else {
        // not a double
        chain_length = best_chain<longer_chain>(copy.begin() + 1, copy.end(),
                                                my_chain_rest).length;
        best.add(chain_length, mv);
      }
    }
    return best.best_move;
  }
private:
  std::string text;
};
