#ifndef TIKTAKTOE_HPP
#define TIKTAKTOE_HPP
#include "esat/math.h"
enum class Ficha
{
    X,
    O,
    V
};

class Tictactoe
{
public:
    const int maxCells = 9;

    //! constructor
    Tictactoe()
    {
        for (int i = 0; i < maxCells; i++)
            celdas_[i] = Ficha::V;
    };

    bool play(int x, int y);
    void resetGame();

    int Tictactoe::turn() const;
    Ficha Tictactoe::winner() const;
    bool isGameEnded() const;
    Ficha Tictactoe::nextPlayer() const;
    Ficha getCell(int x, int y) const;
    int indexarCelda(int x, int y) const;
    esat::Vec2 desindexarCelda(int index) const;

private:
    void setCell(int x, int y, Ficha ficha);
    Ficha winCondition() const;
    Ficha celdas_[3 * 3];
};
#endif

// cl /nologo /Zi /EHs /MD /I"C:\Users\beltr\Desktop\ESAT\SEGUNDO\2doESAT\prog_avanzada\ejercicio_clase\tiktaktoe\version_grafica\Lib_Graph\ESAT_rev248\include" main_console.cpp tiktaktoe.cpp game_interface.cpp iconsole.cpp "C:\Users\beltr\Desktop\ESAT\SEGUNDO\2doESAT\prog_avanzada\ejercicio_clase\tiktaktoe\version_grafica\Lib_Graph\ESAT_rev248\bin\ESAT.lib" opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib /Fe:main_console.exe