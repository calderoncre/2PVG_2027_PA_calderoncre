#pragma once
#ifndef UNO_INTERFACES_IDECK_HPP
#define UNO_INTERFACES_IDECK_HPP

#include "ICard.hpp"
#include "vector"

class IDeck
{
public:
    //! A implementar
    virtual ~IDeck() = default;
    virtual int size() const = 0;
    virtual const ICard &operator[](int index) const = 0;

    // virtual void shuffle() = 0;
    // virtual void swap(IDeck &other) = 0; // mazo de descarte con mazo de pila

    // virtual void move(IDeck &other, int destination = -1) = 0;
    // virtual void move(IDeck &other, int origin, int destination = -1) = 0;
};

class Deck : public IDeck
{
private:
    std::vector<Card> cards_;

public:
    std::vector<Card> &get_cards();
    Card &randomCardinDeck();
    bool cardIsPossible(Type type, Color color) const;
    const Card &getLastCard() const;
    bool isDeckEmpty() const;
    void addCard(Card card);
    Card extractLastCard();
    Card extractCard(int index);
    void barajar_Reordenar();
    int getSize() const;
    Card getCard(int i) const;

    //! A implementar
    virtual int size() const override;
    virtual const ICard &operator[](int index) const override;
    // virtual void shuffle() override;
    // virtual void swap(IDeck &other) override;

    // virtual void move(IDeck &other, int destination = -1) override;
    // virtual void move(IDeck &other, int origin, int destination = -1) override;
    Deck() {

    };
    Deck(Deck &source, int count)
    {
        // construir un mazo a partir de otro
        for (int i = 0; i < count; i++)
        {
            Card carta = source.extractLastCard();
            addCard(carta);
        }
    };
    ~Deck() {
        // = default no 0.
    };
};

#endif // UNO_INTERFACES_IDECK_HPP