#pragma once
#ifndef HAND_VALUE_H
#define HAND_VALUE_H
#include <span>
#include "blackjack/card.hpp"
namespace blackjack
{
    inline int handValue(std::span<const Card> cards)
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
}
#endif