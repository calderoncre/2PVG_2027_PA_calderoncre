#include "IDeck.hpp"
#include "cassert"
std::vector<Card> &Deck::get_cards()
{
    return cards_;
}
int Deck::getSize() const
{
    return static_cast<int>(cards_.size());
}

int Deck::size() const
{
    return static_cast<int>(cards_.size());
}

const ICard &Deck::operator[](int index) const
{
    return cards_[index];
}

void Deck::addCard(Card card)
{
    cards_.push_back(card);
}

Card Deck::extractLastCard()
{
    assert(!cards_.empty());
    Card card = cards_.back();
    cards_.pop_back();
    return card;
}
Card Deck::extractCard(int index)
{
    assert(index >= 0 && index < static_cast<int>(cards_.size()));
    Card card = cards_[index];
    cards_.erase(cards_.begin() + index);
    return card;
}

bool Deck::cardIsPossible(Type type, Color color) const
{
    if (type == Type::Ctmas4 || type == Type::Cambiar_color) // salvar casos de +4 y cambiar color
    {
        if (color != Color::Null)
            return false;
    }
    else
    {
        if (color == Color::Null)
            return false;
    }
    return true;
}
void Deck::barajar_Reordenar()
{
    for (int i = 0; i < getSize(); i++)
    {
        Card &card1 = randomCardinDeck();
        Card &card2 = randomCardinDeck();
        std::swap(card1, card2);
    }
}
Card &Deck::randomCardinDeck()
{
    assert(!cards_.empty());
    int n_random = rand() % getSize();
    return cards_[n_random];
}