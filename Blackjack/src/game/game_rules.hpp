#pragma once
#ifndef GAME_RULES_H
#define GAME_RULES_H
#include "blackjack/irules.hpp"
using namespace blackjack;
namespace blackjack
{
    class GameRules : public IRules
    {
    private:
    public:
        /** @return Short name shown in the menus. */
        const char *name() const override;

        /** @return Decks in the shoe (R1). */
        int deckCount() const override;

        /** @return true if the four 10s of each deck are removed. */
        bool spanishDecks() const override;

        /**
         * @param index 0 for the card dealt after the players' first cards, 1 for
         *        the one after their second cards.
         * @return How the dealer receives that card.
         */
        OpeningCard openingDealerCard(int index) const override;

        /**
         * @param dealer_up The dealer's face-up cards after the deal.
         * @return true if insurance is offered (R13).
         */
        bool offersInsurance(std::span<const Card> dealer_up) const override;

        /**
         * @param dealer_up The dealer's face-up cards after the deal.
         * @return true if the dealer checks for a natural before the players act.
         *         A natural found then ends the round.
         */
        bool dealerPeeks(std::span<const Card> dealer_up) const override;

        /**
         * @param hand A hand whose turn is not finished.
         * @param chips Chips the seat has left.
         * @param hand_count Hands the seat has now.
         * @param action Action to check.
         * @return true if the action is allowed. For Action::Buy the model also
         *         requires a non-empty buyRange().
         */
        bool isAllowed(const HandState &hand, int chips, int hand_count, Action action) const override;

        /**
         * @param hand The hand.
         * @param chips Chips the seat has left.
         * @param min_bet Table minimum.
         * @return Amounts accepted by Action::Buy; min > max if none is.
         */
        BuyRange buyRange(const HandState &hand, int chips, int min_bet) const override;

        /**
         * @brief Asked when a hand starts its turn and after every card or double
         *        it receives.
         * @param hand The hand.
         * @return true if the hand can take no more decisions (bust, 21, split
         *         Aces, a finished double...).
         */
        bool endsTurn(const HandState &hand) const override;

        /**
         * @param hand A hand for which Action::Surrender is allowed; a doubled
         *        hand rescues its double.
         * @return Chips returned to the player when it surrenders or rescues.
         */
        int surrenderRefund(const HandState &hand) const override;

        /**
         * @param hand A finished player hand.
         * @return true if its result does not depend on the dealer's final hand
         *         (R23).
         */
        bool decidedBeforeDealer(const HandState &hand) const override;

        /**
         * @param dealer All the dealer's cards, face up.
         * @param players_waiting true if some player hand still depends on the
         *        dealer's final hand.
         * @return true if the dealer must take another card (R21-R24).
         */
        bool dealerDraws(std::span<const Card> dealer, bool players_waiting) const override;

        /**
         * @param hand A finished player hand, neither surrendered nor rescued.
         * @param dealer All the dealer's cards.
         * @return Chips returned to the player, bet included; 0 if lost (R25).
         */
        int payout(const HandState &hand, std::span<const Card> dealer) const override;
    };
}

#endif