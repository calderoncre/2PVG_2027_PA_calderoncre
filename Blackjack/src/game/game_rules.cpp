#include "game_rules.hpp"
/** @return Short name shown in the menus. */
const char *GameRules::name() const { return "s"; }

int GameRules::deckCount() const { return 0; }

bool GameRules::spanishDecks() const { return "s"; }

OpeningCard GameRules::openingDealerCard(int index) const
{
    if (index == 0)
        return OpeningCard::FaceDown;
    if (index == 1)
        return OpeningCard::FaceUp;
    else
        return OpeningCard::None;
}

bool GameRules::offersInsurance(std::span<const Card> dealer_up) const { return false; }

bool GameRules::dealerPeeks(std::span<const Card> dealer_up) const { return false; }

bool canSplit(const HandState &hand, int chips)
{
    return hand.cards[0].rank == hand.cards[1].rank && chips >= hand.stake;
}
bool GameRules::isAllowed(const HandState &hand, int chips, int hand_count, Action action) const
{
    switch (action)
    {
    case Action::None:
        printf("trying to apply() invalid action\n");
        return false;
    case Action::Hit:
        return true;
    case Action::Stand:
        return true;
    case Action::Double:
        return true;
    case Action::Split:
        return (canSplit(hand, chips));
    default:
        break;
    }
    return false;
}

BuyRange GameRules::buyRange(const HandState &hand, int chips,
                             int min_bet) const { return {}; }

int handValue(std::span<const Card> cards)
{
    int total = 0;
    int ases = 0;

    for (const Card &card : cards)
    {
        int valor = static_cast<int>(card.rank);

        if (card.rank == Rank::Ace)
        {
            total += 11;
            ++ases;
        }
        else
        {
            total += valor > 10 ? 10 : valor;
        }
    }

    while (total > 21 && ases > 0)
    {
        total -= 10; // un as pasa de valer 11 a valer 1
        --ases;
    }

    return total;
}

bool GameRules::endsTurn(const HandState &hand) const
{
    return handValue(hand.cards) >= 21 || hand.doubled;
}

int GameRules::surrenderRefund(const HandState &hand) const
{
    return 0;
}

bool GameRules::decidedBeforeDealer(const HandState &hand) const
{
    return handValue(hand.cards) > 21;
}

bool GameRules::dealerDraws(std::span<const Card> dealer, bool players_waiting) const
{
    return players_waiting && handValue(dealer) < 17;
}

int GameRules::payout(const HandState &hand, std::span<const Card> dealer) const
{
    return 0;
}