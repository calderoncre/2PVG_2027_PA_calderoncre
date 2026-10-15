#include "tiktaktoe.hpp"
#include "game_interface.hpp"

//! main consola
int main(int, char **)
{
    Tictactoe ttt;
    IConsole console_interface;
    console_interface.printBoard(ttt);
    int x = 1, y = 1;
    Ficha ganador = Ficha::V;

    console_interface.main_run(x, y, ttt);
    return 0;
}