#pragma once
#ifndef DEFAULT_CONTROLLER_H
#define DEFAULT_CONTROLLER_H 1
#include "blackjack/icontroller.hpp"
#include "../esat/esatView.hpp"
#include "blackjack_model.hpp"
#include "game_Phases.hpp"
#include "game_rules.hpp"
using namespace blackjack;
namespace blackjack
{
    class Controller : public IController
    {
    private:
        bool step() override;
        /** @return true once no more rounds will be played. */
        bool isGameOver() const override;
        bool hasBetAt(const int seat) const;
        void limpiarInitialBets();
        void processEvents();

        GameRules rules;
        int bets[kMaxSeats]{};
        BlackjackModel model{rules};
        EsatView vista;
        Game_Phases showing_phase = Game_Phases::APUESTAS;
        double inicio_espera = 0.0;

        int mano_iniciada;

    public:
        void update();
        Controller() : mano_iniciada{false}
        {
        }
    };
}
#endif
