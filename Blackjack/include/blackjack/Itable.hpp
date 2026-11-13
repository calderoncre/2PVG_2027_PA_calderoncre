/**
 * @file table.hpp
 * @brief Read-only view of the table: what anyone sitting at it can see.
 *
 * Players and views receive the model through this interface. It never
 * shows face-down dealer cards or the order of the shoe.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_ITABLE_H_
#define BLACKJACK_ITABLE_H_ 1

#include <span>

#include "blackjack/action.hpp"
#include "blackjack/card.hpp"
#include "blackjack/hand_state.hpp"

namespace blackjack
{

  /**
   * @brief Public state of the table.
   *
   * Seats are numbered from 0 (the dealer's left) to seatCount() - 1. Hands of
   * a seat are numbered from 0; a seat has more than one hand after a split.
   */
  class ITable
  {
  public:
    virtual ~ITable() = default;

    /** @return Name of the rule set in play. */
    virtual const char *rulesName() const = 0;

    /** @return Table minimum bet (R7). */
    virtual int minBet() const = 0;

    /** @return Table maximum bet (R7). */
    virtual int maxBet() const = 0;

    /** @return Undealt cards, to turn a running count into a true count. */
    virtual int cardsLeftInShoe() const = 0;

    /** @return Number of seats, from 1 to 7. */
    virtual int seatCount() const = 0;

    virtual int currentSeat() const = 0; // jugador jugando
    virtual int currentHand() const = 0; // mano pendiente de decision

    /**
     * @param seat Seat index.
     * @return Chips the seat owns, not counting bets on the table.
     */
    virtual int chips(int seat) const = 0;

    /**
     * @param seat Seat index.
     * @return Hands of the seat this round: 0 if it sits out, more after a
     *         split.
     */
    virtual int handCount(int seat) const = 0;

    /**
     * @param seat Seat index.
     * @param hand Hand index.
     * @return State of the hand.
     */
    virtual HandState hand(int seat, int hand) const = 0;

    /**
     * @return The dealer's face-up cards, in the order dealt. Valid until the
     *         model changes.
     */
    virtual std::span<const Card> dealerUpCards() const = 0;

    /** @return Number of face-down dealer cards. */
    virtual int dealerHiddenCount() const = 0;

    /**
     * @param seat Seat index.
     * @return Net chips won or lost with insurance once the round is
     *         settled; 0 before, or without insurance.
     */
    virtual int insuranceResult(int seat) const = 0;

    /**
     * @param seat Seat index.
     * @param hand Hand index.
     * @param action Action to check.
     * @return true if the rules allow that action on that hand right now.
     */
    virtual bool isAllowed(int seat, int hand, Action action) const = 0;

    /**
     * @param seat Seat index.
     * @param hand Hand index.
     * @return Amounts accepted by Action::Buy on that hand.
     */
    virtual BuyRange buyRange(int seat, int hand) const = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_TABLE_H_
