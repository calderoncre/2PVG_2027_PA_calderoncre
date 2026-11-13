/**
 * @file card.hpp
 * @brief Cards of a French deck, shared by every component of the game.
 *
 * Part of the common contract: bots may only include headers from
 * include/blackjack/.
 *
 * @author Arnau Rossello
 * @date 2026-10-15
 */
#pragma once
#ifndef BLACKJACK_CARD_H_
#define BLACKJACK_CARD_H_ 1

namespace blackjack
{

  /** @brief Suit of a card. Suits only matter for the Spanish 21 bonuses. */
  enum class Suit
  {
    Clubs,
    Diamonds,
    Hearts,
    Spades
  };

  /**
   * @brief Rank of a card.
   *
   * The underlying value of Two..Ten is the pip count.
   */
  enum class Rank
  {
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
  };

  /** @brief A playing card. Plain data, freely copyable. */
  struct Card
  {
    Rank rank; ///< Rank of the card.
    Suit suit; ///< Suit of the card.
  };

  /** @brief Number of ranks in a deck. */
  inline const int kRankCount = 13;

  /** @brief Number of suits in a deck. */
  inline const int kSuitCount = 4;

} // namespace blackjack

#endif // BLACKJACK_CARD_H_
