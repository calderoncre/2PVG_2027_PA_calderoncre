#include "tiktaktoe.hpp"
#include "game_interface.hpp"
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/time.h"
#include "esat/input.h"

int esat::main(int argc, char **argv)
{
    esat::WindowInit(800, 1000);

    Tictactoe ttt;
    IGraphics graphics_interface;
    graphics_interface.printBoard(ttt);
    int x = 1, y = 1;
    Ficha ganador = Ficha::V;

    graphics_interface.main_run(x, y, ttt);

    esat::WindowDestroy();
    return 0;
}
