/**
 * @file model.hpp
 * @brief Interface of the model: the table state and the game mechanics.
 *
 * The model deals, moves chips and applies moves, asking the IRules it was
 * given about everything the variant decides. It does no input or output
 * and does not decide whose turn it is: the controller does, calling the
 * operations below in the order of a round.
 *
 * A reasonable constructor would be:
 *
 *     Model(const IRules& rules, const TableSettings& settings);
 *
 * which shuffles a new shoe; the rules must outlive the model.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_IMODEL_H_
#define BLACKJACK_IMODEL_H_ 1

#include <stdint.h>

#include "blackjack/action.hpp"
#include "blackjack/card.hpp"
#include "blackjack/irules.hpp"
#include "blackjack/itable.hpp"

namespace blackjack
{

  /** @brief Maximum number of seats at a table (R6). */
  inline const int kMaxSeats = 7;

  /** @brief Settings of a new table. */
  struct TableSettings
  {
    int seat_count;     ///< Number of seats, 1 to kMaxSeats.
    int starting_chips; ///< Chips every seat starts with.
    int min_bet;        ///< Table minimum (R7).
    int max_bet;        ///< Table maximum (R7).
    uint64_t seed;      ///< Seed of the shuffles.
  };

  /** @brief What happened in the model. */
  enum class ModelEventType
  {
    Dealt,         ///< A card was dealt face up.
    DealtFaceDown, ///< A card was dealt face down to the dealer.
    TurnedUp,      ///< A face-down dealer card was turned up.
    Shuffled       ///< The shoe was reshuffled (R11, R29).
  };

  /** @brief Something the model did, reported once through popEvent(). */
  struct ModelEvent
  {
    ModelEventType type; ///< What happened.
    Card card;           ///< The card; not set for DealtFaceDown, Shuffled.
    int seat;            ///< Seat index, or kDealerSeat.
    int hand;            ///< Hand index; for the dealer, the card index.
  };

  /** @brief Seat value of ModelEvent for the dealer. */
  inline const int kDealerSeat = -1;

  /** @brief Result of IModel::peek(). */
  enum class PeekResult
  {
    NoPeek, ///< The variant does not peek now.
    Clear,  ///< The dealer peeked and has no natural.
    Natural ///< The dealer has a natural: it is turned up, the round ends.
  };

  /** @brief The table state plus the operations that change it. */
  class IModel : public ITable
  {
  public:
    // 1. Round start and deal.

    /**
     * @brief Discards the previous round, reshuffles if the cut card has come
     *        out (R11, R28) and takes the bets (R7).
     * @param bets One bet per seat; 0, or an invalid bet, sits the seat out.
     */
    virtual void startRound(const int *bets) = 0;

    /**
     * @brief Deals the next opening card in the order of R12. Call it until
     *        it returns false.
     * @return false when the deal was already complete (nothing dealt).
     */
    virtual bool dealNext() = 0;

    // 2. Insurance and peek.

    /**
     * @param seat Seat index.
     * @return true if insurance is offered to that seat and it can pay it.
     */
    virtual bool insuranceOffered(int seat) const = 0;

    /**
     * @brief Places the insurance side bet: half the stake (R14).
     * @param seat A seat for which insuranceOffered() is true.
     */
    virtual void takeInsurance(int seat) = 0;

    /**
     * @brief Lets the dealer check for a natural, if the variant does.
     * @return What the peek found.
     */
    virtual PeekResult peek() = 0;

    // 3. Player turns, in seat order and hand order.

    /**
     * @brief Starts a hand's turn: deals the second card of a split hand
     *        (R17) and finishes it if it needs no decision.
     * @param seat Seat index.
     * @param hand Hand index.
     */
    virtual void startHand(int seat, int hand) = 0;

    /**
     * @brief Applies a move to a hand. Action::Surrender on a doubled hand
     *        is a rescue (Spanish 21).
     * @param seat Seat index.
     * @param hand Hand index.
     * @param move The move.
     * @return false if the move is not allowed; nothing changes then.
     */
    virtual bool apply(int seat, int hand, const Move &move) = 0;

    // 4. Dealer turn and settlement.

    /**
     * @brief Performs the dealer's next action: turn up the face-down cards,
     *        or draw one card. Call it until it returns false.
     * @return false when the dealer has nothing more to do.
     */
    virtual bool playDealer() = 0;

    /**
     * @brief Pays every hand and insurance; the results are then in
     *        hand(seat, hand).result and insuranceResult(seat).
     */
    virtual void settle() = 0;

    // Reporting, at any time.

    /**
     * @brief Takes the oldest event not reported yet.
     * @param event Out: the event.
     * @return false if there is none.
     */
    virtual bool popEvent(ModelEvent *event) = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_MODEL_H_
