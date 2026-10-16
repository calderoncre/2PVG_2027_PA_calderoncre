#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <conio.h>
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/time.h"
#include "tiktaktoe.hpp"
#include "game_flow.hpp"
// TODO ICONSOLE FUNCTIONS
// _____________________________
// PARA COMPILAR:
// cl /nologo /Zi /EHs /MD /I"C:\Users\beltr\Desktop\ESAT\SEGUNDO\2doESAT\prog_avanzada\ejercicio_clase\tiktaktoe\version_grafica\Lib_Graph\ESAT_rev248\include" main_console.cpp tiktaktoe.cpp game_interface.cpp iconsole.cpp "C:\Users\beltr\Desktop\ESAT\SEGUNDO\2doESAT\prog_avanzada\ejercicio_clase\tiktaktoe\version_grafica\Lib_Graph\ESAT_rev248\bin\ESAT.lib" opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib /Fe:main_console.exe

void IConsole::printBoard(Tictactoe &ttt) const
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%s ", game_messages.fichaTo_string(ttt.getCell(i, j)).c_str());
            if (j == 2)
                printf("\n");
        }
    }
}
int char_to_int(char c)
{
    return c - '0';
}

void IConsole::askPlayer(int &x, int &y, Tictactoe &ttt)
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

void IConsole::printMessages()
{
    if (!game_messages.active_message_)
        return;
    if (esat::Time() - game_messages.message_time_ < 3000.0f)
    {
        printf("%s \n", game_messages.current_message_.c_str());
    }
    else
    {
        game_messages.message_time_ = 0.0f;
        game_messages.active_message_ = false;
    }
}

void IConsole::main_run(int x, int y, Tictactoe &ttt)
{
    // //! falta razonar
    while (1)
    {
        printMessages();
        switch (game_state)
        {
        case GET_VERIFY_MOVE:
            if (_kbhit()) // keyboard hit
            {
                _getch(); // limpiar bufer
                // TODO el usuario ha pulsado una tecla
                askPlayer(x, y, ttt);
                game_state = PROCESS_MOVE;
            }
            break;
        case PROCESS_MOVE:
            if (ttt.play(x, y))
                game_messages.activateMessage(game_messages.rightPlayMessage());
            else
                game_messages.activateMessage(game_messages.badPlayMessage());

            if (ttt.isGameEnded())
                game_state = GAME_FINISHED;
            else
                game_state = SHOW_BOARD;

            break;
        case SHOW_BOARD:
            system("cls");
            printBoard(ttt);
            printMessages();
            if (_kbhit())
                game_state = GET_VERIFY_MOVE;
            break;
        case GAME_FINISHED:
            game_messages.activateMessage(game_messages.winnerMessage(ttt.winner()));
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
