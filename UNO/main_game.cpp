#include <cstdio>
#include <time.h>
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "IController.hpp"

int esat::main(int argc, char **argv)
{
    // srand(time(NULL));
    esat::WindowInit(1000, 1000);
    esat::WindowSetMouseVisibility(true);

    Controller controller;
    controller.init_game();
    // igraphics.LoadSprites();

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        controller.run();

        esat::DrawEnd();
        esat::WindowFrame();
    }
    // igraphics.ReleaseSprites();
    esat::WindowDestroy();
    return 0;
}