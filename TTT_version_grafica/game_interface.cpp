#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <conio.h>

#include "tiktaktoe.hpp"
#include "game_interface.hpp"

// en archivo de compilacion std:c++20 para abrir filtros de hace un tiempo
// TODO SHARED FUNCTIONS BETWEETN ICONSOLE & IGRAPHICS

std::string Game_inteface::rightPlayMessage() const
{
    return "Token placed, right position \n";
}
std::string Game_inteface::badPlayMessage() const
{
    return "Can't place token, wrong position \n";
}
std::string Game_inteface::winnerMessage(const Ficha f) const
{
    if (f == Ficha::V)
        return "We have a draw!!";
    else
        return "Player " + fichaTo_string(f) + " wins!!";
}

std::string Game_inteface::fichaTo_string(Ficha f) const
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
