/**
 * @file player.hpp
 * @brief Interface every player (human or bot) implements.
 *
 * The controller polls a player once per step. A player that has not decided
 * yet returns the "not yet" value (kBetPending, Answer::None,
 * Action::None) and is asked again on the next step, so the controller
 * never blocks waiting for input.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_IPLAYER_H_
#define BLACKJACK_IPLAYER_H_ 1

#include <span>

#include "blackjack/action.hpp"
#include "blackjack/card.hpp"
#include "blackjack/hand_state.hpp"
#include "blackjack/itable.hpp"

namespace blackjack
{

    /**
     * @brief A player sitting at one seat of the table: answers the questions
     *        the controller asks.
     *
     * A player that also wants to follow the game (to count cards, keep
     * statistics...) implements IGameObserver as well, and the program
     * registers it as an observer of the controller.
     */
    class IPlayer
    {
    public:
        virtual ~IPlayer() = default;

        /** @return Display name; stays valid while the player exists. */
        virtual const char *name() const = 0;

        /**
         * @brief Asks for the bet of a new round (R7).
         *
         * The bet must be even, between table.minBet() and table.maxBet(), and
         * not larger than table.chips(seat). An invalid bet sits the player out.
         *
         * @param table The table.
         * @param seat Seat of this player.
         * @return The bet, kSitOut, or kBetPending if still deciding.
         */
        virtual int bet(const ITable &table, int seat) = 0;

        /**
         * @brief Asks whether to take insurance (R13, R14).
         * @param table The table.
         * @param seat Seat of this player.
         * @return Answer::Yes, Answer::No, or Answer::None if still deciding.
         */
        virtual Answer insurance(const ITable &table, int seat) = 0;

        /**
         * @brief Asks for the next action on one of the player's hands.
         *
         * @param table The table.
         * @param seat Seat of this player.
         * @param hand Index of the hand to play.
         * @param state The hand to play.
         * @param dealer_up The dealer's face-up cards, in the order dealt
         *        (empty in Pontoon, where both are face down).
         * @return The move, or a move with Action::None if still deciding.
         */
        virtual Move play(const ITable &table, int seat, int hand,
                          const HandState &state,
                          std::span<const Card> dealer_up) = 0;
    };

} // namespace blackjack

#endif // BLACKJACK_PLAYER_H_
