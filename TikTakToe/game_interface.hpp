#ifndef GAMEINTERFACE_HPP
#define GAMEINTERFACE_HPP
#include <string>
// imprimo tablero
class Game_inteface
{

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

public:
    // constructor
    Game_inteface() : game_state{SHOW_BOARD} {};

    void printBoard(Tictactoe &ttt) const;
    std::string winnerMessage(const Ficha f) const;
    std::string rightPlayMessage() const;
    std::string badPlayMessage() const;
    std::string fichaTo_string(Ficha f) const;
    void askPlayer(int &x, int &y);
    void main_run(int x, int y, Tictactoe &ttt);
    Tictactoe ttt;
    game_states game_state;
    void printMessages(std::string message) const;
};

#endif