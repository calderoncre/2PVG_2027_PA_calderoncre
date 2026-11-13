/**
 * @file controller.hpp
 * @brief Interface of the controller: runs the rounds.
 *
 * The controller decides the order of the round (bets, deal, insurance,
 * peek, player turns, dealer, settlement), asks the players and tells the
 * observer what happened. It does not know which view, if any, is in use.
 *
 * An implementation is ready as soon as it is constructed (RAII). Its
 * constructor should be something like:
 *
 *     Controller(IModel& model, IPlayer* const* players,
 *                IGameObserver* const* observers, int observer_count,
 *                int max_rounds);
 *
 * with one player per seat of the model, the observers that receive every
 * event in that order (the view, and any player that also implements
 * IGameObserver), and max_rounds 0 for no limit. All of them must outlive
 * the controller.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_ICONTROLLER_H_
#define BLACKJACK_ICONTROLLER_H_ 1

#include "blackjack/event.hpp"
#include "blackjack/imodel.hpp"
#include "blackjack/iplayer.hpp"

namespace blackjack
{

  /** @brief Advances the game one event at a time, without blocking. */
  class IController
  {
  public:
    virtual ~IController() = default;

    /**
     * @brief Performs the next event. A player that has not decided yet is
     *        asked again on the next call.
     * @return true if something happened, false if waiting for a player or
     *         the game is over.
     */
    virtual bool step() = 0;

    /** @return true once no more rounds will be played. */
    virtual bool isGameOver() const = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_CONTROLLER_H_
