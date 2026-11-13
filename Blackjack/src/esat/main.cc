#include <cstdio>
#include <time.h>
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"

#include "esatView.hpp"
#include "../game/blackjack_model.hpp"

#include "../game/default_controller.hpp"
using namespace blackjack;
namespace esat
{
    int main(int, char **)
    {
        // srand(time(NULL));
        // size of board png
        esat::WindowInit(1672, 941);
        esat::WindowSetMouseVisibility(true);

        {
            // La vista debe liberar sus sprites antes de destruir la ventana.
            Controller controller;

            while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
            {
                esat::DrawBegin();
                esat::DrawClear(0, 0, 0);

                controller.update();

                esat::DrawEnd();
                esat::WindowFrame();
            }
        }
        // igraphics.ReleaseSprites();
        esat::WindowDestroy();

        return 0;
    }

} // namespace esat
