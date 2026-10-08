
#include <cstdio>
#include <cassert>
#include "tiktaktoe.hpp"

bool Tictactoe::play(int x, int y)
{
    assert(x >= 0 && x < 3 && "'x' out of range");
    assert(y >= 0 && y < 3 && "'y' out of range");
    if (isGameEnded())
        return false;
    if (Ficha::V != getCell(x, y))
        return false;

    setCell(x, y, nextPlayer());
    return true;
}
int Tictactoe::turn() const
{
    int played = 0;
    for (int i = 0; i != maxCells; i++)
    {
        if (celdas_[i] != Ficha::V)
        {
            played++;
        }
    }
    return played;
}

Ficha Tictactoe::nextPlayer() const
{
    return (turn() % 2) ? Ficha::O : Ficha::X;
}

int Tictactoe::indexarCelda(int x, int y) const
{
    const int n_columnas = 3;
    return (n_columnas * y + x);
}

Ficha Tictactoe::getCell(int x, int y) const
{
    return (celdas_[indexarCelda(x, y)]);
}

void Tictactoe::setCell(int x, int y, Ficha ficha)
{
    celdas_[indexarCelda(x, y)] = ficha;
}

bool Tictactoe::isGameEnded() const
{
    if (turn() > maxCells)
        printf("maricon");
    return (turn() >= maxCells || winCondition() != Ficha::V);
}

Ficha Tictactoe::winner() const
{
    return winCondition();
}

Ficha Tictactoe::winCondition() const // gana X, gana O, no gana nadie
{
    // 0 1 2
    // 3 4 5
    // 6 7 8
    // columnas diferenciia de 3
    // filas +3
    // 0 4 8  2 4 6  diagonales
    int n_rows_cols = 3;
    for (int i = 0; i < n_rows_cols; i++)
    {
        // TODO comprobar horizontales
        if (celdas_[i * 3] == Ficha::X && celdas_[(i * 3) + 1] == Ficha::X && celdas_[(i * 3) + 2] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i * 3] == Ficha::O && celdas_[(i * 3) + 1] == Ficha::O && celdas_[(i * 3) + 2] == Ficha::O)
            return Ficha::O;
        // TODO comprobar verticales
        if (celdas_[i] == Ficha::X && celdas_[i + 3] == Ficha::X && celdas_[i + 6] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i] == Ficha::O && celdas_[i + 3] == Ficha::O && celdas_[i + 6] == Ficha::O)
            return Ficha::O;
    }
    // TODO comprobar diagonales
    int n_diagonals = 2;
    for (int i = 0; i < n_diagonals; i++)
    {
        if (celdas_[i * 2] == Ficha::X && celdas_[4] == Ficha::X && celdas_[8 - i * 2] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i * 2] == Ficha::O && celdas_[4] == Ficha::O && celdas_[8 - i * 2] == Ficha::O)
            return Ficha::O;
    }
    return Ficha::V;
}

void Tictactoe::resetGame()
{
    //*this = Tictactoe();
    for (int i = 0; i < maxCells; i++)
        celdas_[i] = Ficha::V;
}
