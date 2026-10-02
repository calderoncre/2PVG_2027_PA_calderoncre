#include <cstdio>
#include <cassert>
enum class Ficha
{
    X,
    O,
    Vacio
};

class Tictactoe
{
public:
    static const int maxCells = 9;

    //! constructor
    Tictactoe() : turn_{0}
    {
        for (int i = 0; i < maxCells; i++)
            celdas_[i] = Ficha::Vacio;
    };

    bool play(int x, int y);
    bool isGameEnded(Ficha &ganador) const;
    Ficha winCondition() const;
    Ficha getCell(int x, int y) const;
    void askPlayer(int &x, int &y);
    void placeToken(int x, int y);
    void printBoard() const;

private:
    Ficha celdas_[3 * 3];
    int turn_;
};

bool Tictactoe::play(int x, int y)
{
    if (getCell(x, y) != Ficha::Vacio)
    {
        printf("Wrong position, try again!\n");
        return false;
    }
    else
    {
        printf("posicion valida \n");
        return true;
    }
}

void Turn()
{
    // las fichas que faltan por poner me dan el turno
    // impar devuelve 1 / par devuelve 0 o 2 lo que quiera
}

void Tictactoe::printBoard() const
{
    for (int i = 0; i < maxCells; i++)
    {
        if (i == 3 || i == 6)
            printf("\n");
        printf("%d ", celdas_[i]);
    }
}

Ficha Tictactoe::getCell(int x, int y) const
{
    return (celdas_[3 * y + x]);
}

void Tictactoe::askPlayer(int &x, int &y)
{
    printf("\n");
    printf("Player %d introduce a number \n", turn_);
    while (scanf("%d", &x) != 1 || scanf("%d", &y) != 1)
    {
        assert(x >= 0 && x < 3);
        assert(y >= 0 && y < 3);
    }
}

void Tictactoe::placeToken(int x, int y)
{
    assert(turn_ >= 0);
    assert(turn_ <= 1);
    if (turn_ != 0)
    {
        celdas_[3 * y + x] = Ficha::O;
        turn_ = 0;
    }
    else
    {
        celdas_[3 * y + x] = Ficha::X;
        turn_ = 1;
    }
}

bool Tictactoe::isGameEnded(Ficha &ganador) const
{
    int counter = 0;
    // si no hay nada vacio
    for (int i = 0; i < maxCells; i++)
    {
        if (celdas_[i] != Ficha::Vacio)
            counter++;
        if (counter == 9)
        {
            printf("Empate! \n");
            return true;
        }
    }
    if (counter < 9)
    {
        ganador = winCondition();
        if (ganador != Ficha::Vacio)
            return true;
    }

    return false;
}

Ficha Tictactoe::winCondition() const // gana X, gana O, no gana nadie
{
    // 0 1 2
    // 3 4 5
    // 6 7 8
    // columnas diferenciia de 3
    // filas +3
    // 0 4 8  2 4 6  diagonales
    int n_rows_cols = 3;
    for (int i = 0; i < n_rows_cols; i++)
    {
        // TODO comprobar horizontales
        if (celdas_[i * 3] == Ficha::X && celdas_[(i * 3) + 1] == Ficha::X && celdas_[(i * 3) + 2] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i * 3] == Ficha::O && celdas_[(i * 3) + 1] == Ficha::O && celdas_[(i * 3) + 2] == Ficha::O)
            return Ficha::O;
        // TODO comprobar verticales
        if (celdas_[i] == Ficha::X && celdas_[i + 3] == Ficha::X && celdas_[i + 6] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i] == Ficha::O && celdas_[i + 3] == Ficha::O && celdas_[i + 6] == Ficha::O)
            return Ficha::O;
    }
    // TODO comprobar diagonales
    int n_diagonals = 2;
    for (int i = 0; i < n_diagonals; i++)
    {
        if (celdas_[i * 2] == Ficha::X && celdas_[4] == Ficha::X && celdas_[8 - i * 2] == Ficha::X)
            return Ficha::X;
        else if (celdas_[i * 2] == Ficha::O && celdas_[4] == Ficha::O && celdas_[8 - i * 2] == Ficha::O)
            return Ficha::O;
    }
    return Ficha::Vacio;
}

void printWinner(const Ficha f)
{
    printf("Winner is player %d", f);
}

int main(int, char **)
{
    Tictactoe ttt;
    ttt.printBoard();
    int x, y;
    Ficha ganador = Ficha::Vacio;
    while (!ttt.isGameEnded(ganador))
    {
        ttt.askPlayer(x, y);
        if (ttt.play(x, y)) // o while
            ttt.placeToken(x, y);
        ttt.printBoard();
    }
    printWinner(ganador);

    return 0;
}