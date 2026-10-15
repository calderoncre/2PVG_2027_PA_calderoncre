#include "tiktaktoe.hpp"
#include "game_interface.hpp"

int main(int, char **)
{
    Tictactoe ttt;
    Game_inteface game_interf;
    game_interf.printBoard(ttt);
    int x = 1, y = 1;
    Ficha ganador = Ficha::V;

    game_interf.main_run(x, y, ttt);
    return 0;
}