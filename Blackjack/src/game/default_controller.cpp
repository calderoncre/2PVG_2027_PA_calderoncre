#include "default_controller.hpp"
#include "esat/input.h"
#include <cstdio>
using namespace blackjack;

bool Controller::step()
{
    return false;
}

bool Controller::isGameOver() const
{
    return false;
}
void Controller::update()
{
    switch (showing_phase)
    {
    case Game_Phases::APUESTAS:
    {
        vista.printAPUESTAS(bets);
        vista.updateAPUESTAS();
        int player_spot = vista.getSelectedPlayerSpot();
        int selected_chip = vista.getSelectedChip();
        if (vista.hasBetSelection() && esat::MouseButtonDown(0))
        {
            const int chip_value = vista.chipIndex_toValue(selected_chip);
            // assert(chip_value == 0 && "chip value valor inesperado");
            const int next_bet = bets[player_spot] + chip_value;

            if (next_bet <= model.chips(player_spot) &&
                next_bet <= model.maxBet())
            {
                bets[player_spot] = next_bet;
            }
        }
        if (vista.get_puede_repartir())
        {
            for (int seat = 0; seat < kMaxSeats; ++seat)
            {
                if (hasBetAt(seat))
                {
                    model.startRound(bets);
                    showing_phase = Game_Phases::REPARTO;
                    break;
                }
            }
        }
        break;
    }
    case Game_Phases::REPARTO:
        if (!model.dealNext())
            showing_phase = Game_Phases::JUEGO;
        vista.printREPARTOS(model);
        vista.draw(model);
        break;
    case Game_Phases::JUEGO:
    {
        vista.updateJUEGO();
        vista.printJUEGO(model);
        vista.draw(model);
        Move action_selected = vista.getActionSelected();
        int current_seat = model.currentSeat();
        int current_hand = model.currentHand();

        // !iniciar la mano
        if (!mano_iniciada)
        {
            model.startHand(current_seat, current_hand);
            mano_iniciada = true;
        }
        const HandState prev_hand = model.hand(current_seat, current_hand);
        if (!prev_hand.finished)
        {
            // !esperar una decisión
            if (action_selected.action != Action::None)
            {
                if (model.apply(current_seat, current_hand, action_selected))
                {
                    const HandState post_hand = model.hand(current_seat, current_hand);
                    if (post_hand.finished)
                    {
                        if (!model.avanzarMano(current_seat))
                        {
                            model.reiniciarHandCount();
                            model.avanzarTurno();
                        }
                        mano_iniciada = false;
                    }
                }
                else
                {
                    printf("Movimiento no permitido\n");
                }
            }
        }
        else
        {
            //! saltar a la siguiente mano pendiente
            //! si no hay manos pendientes pasar de turno
            if (!model.avanzarMano(current_seat))
                model.avanzarTurno();
            mano_iniciada = false;
        }
        if (model.getTurn() >= model.getnActivePlayers())
            showing_phase = Game_Phases::DEALER;
        break;
    }
    case Game_Phases::DEALER:
        vista.updateJUEGO();
        vista.printJUEGO(model);
        vista.draw(model);
        if (!model.playDealer())
            showing_phase = Game_Phases::LIQUIDAR_APUESTAS;

        break;
    case Game_Phases::LIQUIDAR_APUESTAS:
        vista.updateJUEGO();
        vista.printJUEGO(model);
        vista.draw(model);

        model.settle();

        break;
    default:
        break;
    }
}

bool Controller::hasBetAt(const int seat) const
{
    return seat >= 0 && seat < kMaxSeats && bets[seat] >= model.minBet() && bets[seat] <= model.maxBet() && bets[seat] <= model.chips(seat);
}
