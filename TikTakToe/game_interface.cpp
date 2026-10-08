#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <conio.h>
#include "tiktaktoe.hpp"
#include "game_interface.hpp"

void Game_inteface::printBoard(Tictactoe &ttt) const
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%s ", fichaTo_string(ttt.getCell(i, j)).c_str()); // !necesita cast!?
            if (j == 2)
                printf("\n");
        }
    }
}
int char_to_int(char c)
{
    return c - '0';
}

void Game_inteface::askPlayer(int &x, int &y)
{
    printf("\n");
    printf("Player %d introduce a row \n", ttt.nextPlayer());
    char row = _getch();
    printf("Player %d introduce a column \n", ttt.nextPlayer());
    char col = _getch();
    if (row < '0' || row > '2' || col < '0' || col > '2')
    {
        printf("You must introduce right numbers, 0, 1, or 2\n");
        // // limpiar buffer
        //! falta comprobar algo mas
    }
    else
    {
        x = char_to_int(row);
        y = char_to_int(col);
        assert(x >= 0 && x < 3 && y >= 0 && y < 3 && "Valores x,y fuera de rango");
    }
}
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
void Game_inteface::printMessages(std::string message) const
{
    printf("%s \n", message.c_str());
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

void Game_inteface::main_run(int x, int y, Tictactoe &ttt)
{
    // //! falta razonar
    while (1)
    {
        switch (game_state)
        {
        case GET_VERIFY_MOVE:
            if (_kbhit()) // keyboard hit
            {
                _getch(); // limpiar bufer
                // TODO el usuario ha pulsado una tecla
                askPlayer(x, y);
                game_state = PROCESS_MOVE;
            }
            break;
        case PROCESS_MOVE:
            if (ttt.play(x, y))
                printMessages(rightPlayMessage());
            else
                printMessages(badPlayMessage());

            if (ttt.isGameEnded())
                game_state = GAME_FINISHED;
            else
                game_state = SHOW_BOARD;

            break;
        case SHOW_BOARD:
            printBoard(ttt);
            system("cls");
            if (_kbhit())
                game_state = GET_VERIFY_MOVE;
            break;
        case GAME_FINISHED:
            printMessages(winnerMessage(ttt.winner()));
            if (_kbhit())
                game_state = RESET;
            break;
        case RESET:
            ttt.resetGame();
            game_state = SHOW_BOARD;
            break;
        default:
            break;
        }
    }

    // 0 = O
    // 1 = X
    // 2 = draw
}