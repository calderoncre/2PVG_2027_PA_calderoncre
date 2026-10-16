#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <conio.h>
#include <esat/time.h>

#include "tiktaktoe.hpp"
#include "game_flow.hpp"

// en archivo de compilacion std:c++20 para abrir filtros de hace un tiempo
// TODO SHARED FUNCTIONS BETWEETN ICONSOLE & IGRAPHICS
void Game_messages::activateMessage(std::string message)
{
    current_message_ = message;
    message_time_ = esat::Time();
    active_message_ = true;
}

std::string Game_messages::rightPlayMessage() const
{
    return "Token placed, right position \n";
}
std::string Game_messages::badPlayMessage() const
{
    return "Can't place token, wrong position \n";
}
std::string Game_messages::winnerMessage(const Ficha f) const
{
    if (f == Ficha::V)
        return "We have a draw!!";
    else
        return "Player " + fichaTo_string(f) + " wins!!";
}

std::string Game_messages::fichaTo_string(Ficha f) const
{
    switch (f)
    {
    case Ficha::X:
        return "|X|";
        break;
    case Ficha::O:
        return "|O|";
        break;
    default:
        return "| |";
        break;
    }
}
