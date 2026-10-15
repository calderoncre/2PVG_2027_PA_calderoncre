#ifndef GAMEINTERFACE_HPP
#define GAMEINTERFACE_HPP
#include <string>
#include "esat/math.h"
// imprimo tablero
class Game_inteface
{

public:
    enum game_states
    {
        // pido jugada
        GET_VERIFY_MOVE,
        // verifico/proceso jugada
        PROCESS_MOVE,
        // muestro resultado de la jugada
        SHOW_BOARD,

        GAME_FINISHED,

        RESET
    };

    // constructor
    Game_inteface() : game_state{GET_VERIFY_MOVE}, active_message_{false}, current_message_{""}, message_time_{0.0f} {};
    // virtual pure methods
    //  the idea is that only childs can apply and use these functions,
    //  in this case IConsole

protected:
    virtual void askPlayer(int &x, int &y, Tictactoe &ttt) = 0;
    virtual void main_run(int x, int y, Tictactoe &ttt) = 0; // porque si va a haber diferenciación de mains
    virtual void printMessages() = 0;
    virtual void printBoard(Tictactoe &ttt) const = 0;

    void activateMessage(std::string message);

    std::string winnerMessage(const Ficha f) const;
    std::string rightPlayMessage() const;
    std::string badPlayMessage() const;
    std::string fichaTo_string(Ficha f) const;

    Tictactoe ttt;
    game_states game_state;

    bool active_message_;
    std::string current_message_;
    double message_time_;
};

class IConsole : public Game_inteface
{
public:
    virtual void askPlayer(int &x, int &y, Tictactoe &ttt) override;
    virtual void main_run(int x, int y, Tictactoe &ttt) override; // porque si va a haber diferenciación de mains
    virtual void printBoard(Tictactoe &ttt) const override;
    virtual void printMessages() override;
};

class IGraphics : public Game_inteface
{
private:
    const int window_width;
    const int window_height;
    const int n_pos;
    esat::Vec2 chip_pos[9];

    void initialize_chip_positions();

    void DebugPoints(esat::Vec2 pos) const;
    bool isMouseInsideCell(int i) const;
    esat::Vec2 checkInput_SaveCell(Tictactoe &ttt) const;
    void drawBoard() const;
    void drawX(esat::Vec2 pos) const;
    void drawCircle(esat::Vec2 pos) const;
    void drawChips(Ficha f, esat::Vec2 pos) const;
    void square300x300(esat::Vec2 pos) const;
    esat::Vec2 getVec_from_rowcol(int row, int col) const;
    esat::Vec2 window_center() const;

public:
    IGraphics() : window_width{800}, window_height{600}, n_pos{9}
    {
        for (int i = 0; i < n_pos; i++)
        {
            chip_pos[i] = {0, 0};
        }
    }
    virtual void printBoard(Tictactoe &ttt) const override;

    virtual void askPlayer(int &x, int &y, Tictactoe &ttt) override;
    virtual void main_run(int x, int y, Tictactoe &ttt) override; // porque si va a haber diferenciación de mains
    virtual void printMessages() override;                        // no puede ser const porque modifico la variable active_message_ ojo con eso
};

#endif