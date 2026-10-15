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
#include "game_interface.hpp"

// TODO IGRAPHICS FUNCTIONS
// _____________________________
void IGraphics::printBoard(Tictactoe &ttt) const
{
    drawBoard();
    esat::DrawText(window_center().x - 250, window_center().y + 600, fichaTo_string(ttt.nextPlayer()).c_str());
    int row, col;
    for (row = 0; row < 3; row++)
    {
        for (col = 0; col < 3; col++)
        {
            if (ttt.getCell(row, col) == Ficha::V)
                continue;
            else
                drawChips(ttt.getCell(row, col), getVec_from_rowcol(row, col));
        }
    }
}

void IGraphics::drawBoard() const
{
    // horizontal

    esat::DrawLine(window_center().x - 300, window_center().y - 100, window_center().x + 300, window_center().y - 100);
    esat::DrawLine(window_center().x - 300, window_center().y + 100, window_center().x + 300, window_center().y + 100);
    // vertical
    esat::DrawLine(window_center().x - 100, window_center().y + 300, window_center().x - 100, window_center().y - 300);
    esat::DrawLine(window_center().x + 100, window_center().y + 300, window_center().x + 100, window_center().y - 300);
}
void IGraphics::drawX(esat::Vec2 pos) const
{
    int offsetx = 10;
    int offsety = 24;
    esat::DrawLine(pos.x - offsetx, pos.y + offsety, pos.x + offsetx, pos.y - offsety);
    esat::DrawLine(pos.x - offsetx, pos.y - offsety, pos.x + offsetx, pos.y + offsety);
}
void IGraphics::drawCircle(esat::Vec2 pos) const
{
    float r = 10;
    float cx = pos.x;
    float cy = pos.y;
    for (int x = -r; x <= r; x++)
    {
        int y = (int)sqrt(r * r - x * x);

        esat::DrawLine(cx + x, cy + y, cx + x + 1, cy + y);
        esat::DrawLine(cx + x, cy - y, cx + x + 1, cy - y);
    }
}
void IGraphics::drawChips(Ficha f, esat::Vec2 pos) const
{
    if (f == Ficha::V)
        return;
    if (f == Ficha::X)
        drawX(pos);
    else
        drawCircle(pos);
}
esat::Vec2 IGraphics::getVec_from_rowcol(int row, int col) const
{
    int offset = 200;
    int space_between_chips = 200;
    esat::Vec2 first_pos = {window_center().x - offset, window_center().y - offset};

    esat::DrawSetStrokeColor(0, 0, 255, 255);

    return {first_pos.x + (row * space_between_chips), first_pos.y + (col * space_between_chips)};
}

esat::Vec2 IGraphics::window_center() const
{
    return {(float)window_width / 2, (float)window_height / 2};
}

void IGraphics::initialize_chip_positions()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            chip_pos[ttt.indexarCelda(i, j)].x = getVec_from_rowcol(i, j).x;
            chip_pos[ttt.indexarCelda(i, j)].y = getVec_from_rowcol(i, j).y;
            DebugPoints(chip_pos[ttt.indexarCelda(i, j)]);
        }
    }
}
void IGraphics::DebugPoints(esat::Vec2 pos) const
{
    esat::DrawSetStrokeColor(255, 0, 0, 255);
    float points[8] =
        {
            pos.x - 2, pos.y + 2,
            pos.x - 2, pos.y - 2,
            pos.x + 2, pos.y - 2,
            pos.x + 2, pos.y + 2};
    esat::DrawSolidPath(points, 4);
}
void IGraphics::askPlayer(int &x, int &y, Tictactoe &ttt)
{
    esat::Vec2 cell = checkInput_SaveCell(ttt);
    // if (ttt.getCell(cell.x, cell.y) != Ficha::V)
    //     return;
    int new_x = cell.x;
    int new_y = cell.y;
    x = new_x;
    y = new_y;
}

esat::Vec2 IGraphics::checkInput_SaveCell(Tictactoe &ttt) const
{
    // return -1 as no chip position
    int nCells = 9;

    for (int i = 0; i < nCells; i++)
    {
        if (isMouseInsideCell(i) && ttt.getCell(ttt.desindexarCelda(i).x, ttt.desindexarCelda(i).y) == Ficha::V)
        {
            square300x300(chip_pos[i]);
            if (esat::IsSpecialKeyDown(esat::kSpecialKey_Space))
            {
                return {ttt.desindexarCelda(i).x, ttt.desindexarCelda(i).y};
            }
        }
    }
    return {-1, -1};
}

bool IGraphics::isMouseInsideCell(int i) const
{
    float mouse_x = esat::MousePositionX();
    float mouse_y = esat::MousePositionY();
    int offset = 100;
    return (mouse_x < (chip_pos[i].x + offset) && mouse_x > (chip_pos[i].x - offset) && mouse_y < (chip_pos[i].y + offset) && (mouse_y > chip_pos[i].y - offset));
}

void IGraphics::square300x300(esat::Vec2 pos) const
{
    int offset = 100;
    float puntos[8] = {
        pos.x - offset, pos.y + offset,
        pos.x - offset, pos.y - offset,
        pos.x + offset, pos.y - offset,
        pos.x + offset, pos.y + offset};
    esat::DrawSolidPath(puntos, 4);
}

void IGraphics::printMessages()
{

    if (!active_message_)
        return;
    double cooldown = 3000.0f;
    esat::DrawSetFillColor(255, 255, 0, 255);
    esat::DrawSetTextSize(20.0f);
    esat::DrawSetTextFont("mi_fuente2.ttf");
    if (esat::Time() - message_time_ < cooldown)
    {
        esat::DrawText(window_center().x - 250, window_center().y + 500, current_message_.c_str());
    }
    else
        active_message_ = false;
}

void IGraphics::main_run(int x, int y, Tictactoe &ttt)
{
    initialize_chip_positions();
    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::SpecialKey(esat::kSpecialKey_Escape)))
    {
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);
        esat::DrawSetStrokeColor(255, 255, 255, 255);

        // do always:
        printBoard(ttt);
        printMessages();

        switch (game_state)
        {
        case GET_VERIFY_MOVE:

            // // TODO el usuario ha pulsado una tecla

            askPlayer(x, y, ttt);
            if (x != -1 && y != -1)
            {
                game_state = PROCESS_MOVE;
            }

            break;
        case PROCESS_MOVE:

            if (ttt.play(x, y))
            {
                activateMessage(rightPlayMessage());
                game_state = GET_VERIFY_MOVE;
            }
            else
                activateMessage(badPlayMessage());

            if (ttt.isGameEnded())
                game_state = GAME_FINISHED;
            else
                game_state = GET_VERIFY_MOVE;

            break;
        case GAME_FINISHED:
            printf("GAME FINISHED \n");
            activateMessage(winnerMessage(ttt.winner()));
            game_state = RESET;
            break;
        case RESET:
            ttt.resetGame();
            printf("RESET \n");
            game_state = GET_VERIFY_MOVE;
            break;
        default:
            break;
        }

        esat::DrawEnd();
        esat::WindowFrame();
    }
}