/**
 * @file view.hpp
 * @brief Interface of a view: shows the table and collects the human's
 *        decisions.
 *
 * An implementation is ready as soon as it is constructed (RAII): its
 * constructor opens the window or console. By convention it is
 *
 *     View(IPlayer* const* players);
 *
 * where players is the seat array the program fills, the view's own
 * human() included; it is only read after construction. A program runs any
 * view with the same loop:
 *
 *     while (!view.quit()) {
 *       view.update(model);
 *       if (!view.busy()) controller.step();
 *       view.draw(model);
 *     }
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_IVIEW_H_
#define BLACKJACK_IVIEW_H_ 1

#include "blackjack/event.hpp"
#include "blackjack/iplayer.hpp"
#include "blackjack/irules.hpp"
#include "blackjack/itable.hpp"

namespace blackjack
{

  /**
   * @brief A front end. It receives the controller's events and owns the
   *        human player, the only player whose decisions come from input.
   */
  class IView : public IGameObserver
  {
  public:
    /**
     * @brief Lets the user pick a rule set. Call it until it returns a value
     *        other than -1 or quit() becomes true.
     * @param rules Available rule sets.
     * @param count Number of rule sets.
     * @return Index of the chosen set, or -1 if none is chosen yet.
     */
    virtual int chooseRules(const IRules *const *rules, int count) = 0;

    /** @return The human player, fed by this view's input. */
    virtual IPlayer *human() = 0;

    /**
     * @brief Reads input and answers what the human player is asked.
     * @param table The table.
     */
    virtual void update(const ITable &table) = 0;

    /**
     * @brief Shows the current state.
     * @param table The table.
     */
    virtual void draw(const ITable &table) = 0;

    /**
     * @return true while the view wants the game to wait, for example to
     *         finish an animation.
     */
    virtual bool busy() const = 0;

    /** @return true once the user wants to leave. */
    virtual bool quit() const = 0;
  };

} // namespace blackjack

#endif // BLACKJACK_VIEW_H_
