#pragma once
#ifndef GAME_PHASES_H
#define GAME_PHASES_H
using namespace blackjack;
namespace blackjack
{
    enum class Game_Phases
    {
        APUESTAS,
        REPARTO,
        JUEGO,
        DEALER,
        LIQUIDAR_APUESTAS
    };
} // namespace blackjack

#endif