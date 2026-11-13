/**
 * @file action.hpp
 * @brief Decisions a player can return to the controller.
 *
 * Every decision has a "not yet" value (Action::None, Answer::None,
 * kBetPending).
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_ACTION_H_
#define BLACKJACK_ACTION_H_ 1

namespace blackjack
{

  /** @brief Action on one hand during the player's turn. */
  enum class Action
  {
    None,      ///< No decision yet: the controller will ask again.
    Hit,       ///< Take one card ("twist" in Pontoon).
    Stand,     ///< End the hand's turn ("stick" in Pontoon).
    Double,    ///< Double the bet and take exactly one card.
    Split,     ///< Split a pair into two hands.
    Surrender, ///< Give up the hand (Spanish 21: surrender or rescue).
    Buy        ///< Pontoon: raise the bet and take one card.
  };

  /** @brief Number of values in Action, None included. */
  inline const int kActionCount = 7;

  /** @brief A yes/no answer, used for insurance. */
  enum class Answer
  {
    None, ///< No decision yet: the controller will ask again.
    Yes,
    No
  };

  /**
   * @brief A decision on a hand: an action plus an amount.
   *
   * The amount is only read for Action::Buy, where it is the extra bet.
   */
  struct Move
  {
    Action action; ///< What to do with the hand.
    int amount;    ///< Extra bet for Action::Buy; ignored otherwise.
  };

  /** @brief Amounts accepted by Action::Buy; min > max when none is. */
  struct BuyRange
  {
    int min; ///< Smallest extra bet.
    int max; ///< Largest extra bet.
  };

  /** @brief Returned by IPlayer::bet() while the player is still deciding. */
  inline const int kBetPending = -1;

  /** @brief Returned by IPlayer::bet() to sit out the round. */
  inline const int kSitOut = 0;

} // namespace blackjack

#endif // BLACKJACK_ACTION_H_
