/**
 * @file event.hpp
 * @brief Events the controller reports to an observer (a view).
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_EVENT_H_
#define BLACKJACK_EVENT_H_ 1

#include "blackjack/action.hpp"
#include "blackjack/card.hpp"
#include "blackjack/itable.hpp"

namespace blackjack
{

  /** @brief Kind of event. Which GameEvent fields are used depends on it. */
  enum class EventType
  {
    RoundStart,      ///< amount = round number.
    Shuffle,         ///< The shoe has been reshuffled.
    Bet,             ///< seat bets amount.
    SitOut,          ///< seat sits out the round.
    CardToPlayer,    ///< seat, hand receive card.
    CardToDealer,    ///< Dealer receives card face up; hand = card index.
    HiddenToDealer,  ///< Dealer receives a face-down card.
    DealerReveal,    ///< Dealer turns card face up; hand = card index.
    Insurance,       ///< seat takes insurance for amount.
    NoInsurance,     ///< seat declines insurance.
    DealerPeekClear, ///< The dealer peeked and has no natural.
    DealerNatural,   ///< The dealer has a natural.
    Action,          ///< seat, hand perform action (amount for buys).
    IllegalMove,     ///< seat, hand tried action; it is asked again.
    HandResult,      ///< seat, hand settle with net amount.
    InsuranceResult, ///< seat's insurance settles with net amount.
    RoundEnd,        ///< The round has been settled; amount = round.
    PlayerBroke,     ///< seat cannot afford the minimum bet any more.
    GameOver         ///< No more rounds; amount = rounds played.
  };

  /** @brief One thing that happened at the table. */
  struct GameEvent
  {
    EventType type; ///< What happened.
    int seat;       ///< Seat involved, or -1.
    int hand;       ///< Hand involved, or -1.
    Card card;      ///< Card involved, if any.
    Action action;  ///< Action involved, if any.
    int amount;     ///< Chips or count involved, if any.
  };

  /**
   * @brief Receives the controller's events. Views implement it, and so does
   *        any player that wants to follow the game.
   *
   * Events never carry a face-down card: HiddenToDealer has no card, and the
   * card arrives with DealerReveal once it is turned up.
   */
  class IGameObserver
  {
  public:
    virtual ~IGameObserver() = default;

    /**
     * @brief Called right after something happens at the table.
     * @param event What happened.
     * @param table The table after the event.
     */
    virtual void onEvent(const GameEvent &event, const ITable &table) = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_EVENT_H_
