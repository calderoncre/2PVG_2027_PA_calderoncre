#ifndef TIKTAKTOE_HPP
#define TIKTAKTOE_HPP
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

private:
    void setCell(int x, int y, Ficha ficha);
    Ficha winCondition() const;
    Ficha celdas_[3 * 3];
};
#endif