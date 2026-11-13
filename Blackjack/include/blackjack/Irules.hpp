/**
 * @file rules.hpp
 * @brief Interface of a rule set (a Blackjack variant).
 *
 * The rules are a policy: they never change the table. The model asks them
 * what the variant decides (which cards the dealer gets, what is allowed,
 * when a turn ends, what a hand pays) and performs the mechanics itself.
 * Every question is answered from public data, so any rule set works with
 * any model.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_IRULES_H_
#define BLACKJACK_IRULES_H_ 1

#include <span>

#include "blackjack/action.hpp"
#include "blackjack/card.hpp"
#include "blackjack/hand_state.hpp"

namespace blackjack
{

  /** @brief How the dealer receives one of the opening cards (R12). */
  enum class OpeningCard
  {
    None,    ///< The dealer gets no card at this position.
    FaceUp,  ///< Dealt face up.
    FaceDown ///< Dealt face down (hole card).
  };

  /** @brief A variant of Blackjack. Stateless: every method is const. */
  class IRules
  {
  public:
    virtual ~IRules() = default;

    /** @return Short name shown in the menus. */
    virtual const char *name() const = 0;

    /** @return Decks in the shoe (R1). */
    virtual int deckCount() const = 0;

    /** @return true if the four 10s of each deck are removed. */
    virtual bool spanishDecks() const = 0;

    /**
     * @param index 0 for the card dealt after the players' first cards, 1 for
     *        the one after their second cards.
     * @return How the dealer receives that card.
     */
    virtual OpeningCard openingDealerCard(int index) const = 0;

    /**
     * @param dealer_up The dealer's face-up cards after the deal.
     * @return true if insurance is offered (R13).
     */
    virtual bool offersInsurance(std::span<const Card> dealer_up) const = 0;

    /**
     * @param dealer_up The dealer's face-up cards after the deal.
     * @return true if the dealer checks for a natural before the players act.
     *         A natural found then ends the round.
     */
    virtual bool dealerPeeks(std::span<const Card> dealer_up) const = 0;

    /**
     * @param hand A hand whose turn is not finished.
     * @param chips Chips the seat has left.
     * @param hand_count Hands the seat has now.
     * @param action Action to check.
     * @return true if the action is allowed. For Action::Buy the model also
     *         requires a non-empty buyRange().
     */
    virtual bool isAllowed(const HandState &hand, int chips, int hand_count,
                           Action action) const = 0;

    /**
     * @param hand The hand.
     * @param chips Chips the seat has left.
     * @param min_bet Table minimum.
     * @return Amounts accepted by Action::Buy; min > max if none is.
     */
    virtual BuyRange buyRange(const HandState &hand, int chips,
                              int min_bet) const = 0;

    /**
     * @brief Asked when a hand starts its turn and after every card or double
     *        it receives.
     * @param hand The hand.
     * @return true if the hand can take no more decisions (bust, 21, split
     *         Aces, a finished double...).
     */
    virtual bool endsTurn(const HandState &hand) const = 0;

    /**
     * @param hand A hand for which Action::Surrender is allowed; a doubled
     *        hand rescues its double.
     * @return Chips returned to the player when it surrenders or rescues.
     */
    virtual int surrenderRefund(const HandState &hand) const = 0;

    /**
     * @param hand A finished player hand.
     * @return true if its result does not depend on the dealer's final hand
     *         (R23).
     */
    virtual bool decidedBeforeDealer(const HandState &hand) const = 0;

    /**
     * @param dealer All the dealer's cards, face up.
     * @param players_waiting true if some player hand still depends on the
     *        dealer's final hand.
     * @return true if the dealer must take another card (R21-R24).
     */
    virtual bool dealerDraws(std::span<const Card> dealer,
                             bool players_waiting) const = 0;

    /**
     * @param hand A finished player hand, neither surrendered nor rescued.
     * @param dealer All the dealer's cards.
     * @return Chips returned to the player, bet included; 0 if lost (R25).
     */
    virtual int payout(const HandState &hand,
                       std::span<const Card> dealer) const = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_RULES_H_
