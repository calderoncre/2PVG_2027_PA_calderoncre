/**
 * @file hand_state.hpp
 * @brief Public state of one hand, as every component sees it.
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_HAND_STATE_H_
#define BLACKJACK_HAND_STATE_H_ 1

#include <span>

#include "blackjack/card.hpp"

namespace blackjack
{

  /**
   * @brief Snapshot of one player hand. Everything in it is public at the
   *        table: player cards are always face up.
   *
   * The cards point into the model, so they are only valid until the model
   * changes.
   */
  struct HandState
  {
    std::span<const Card> cards; ///< Cards, first dealt first.
    int stake;                   ///< Bet placed when the hand was created.
    int bet;                     ///< Total bet, doubles and buys included.
    int result;                  ///< Net chips won or lost; 0 before settling.
    bool from_split;             ///< Made by splitting a pair.
    bool split_aces;             ///< Made by splitting Aces.
    bool doubled;                ///< Has doubled down.
    bool twisted;                ///< Has taken a card with Hit.
    bool surrendered;            ///< Surrendered (late surrender).
    bool rescued;                ///< Rescued a double (Spanish 21).
    bool finished;               ///< Its turn is over.
  };

} // namespace blackjack

#endif // BLACKJACK_HAND_STATE_H_
