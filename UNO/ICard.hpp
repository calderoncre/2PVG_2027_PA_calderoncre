#pragma once
#ifndef UNO_INTERFACES_ICARD_HPP
#define UNO_INTERFACES_ICARD_HPP

#include "Color.hpp"
#include "Type.hpp"

class ICard
{
public:
    virtual Color getColor() const = 0;
    virtual Type getType() const = 0;
    // virtual int getValue() = 0; // Undefined if isSpecial = true
    virtual bool isSpecial() = 0;
    virtual bool skipsPlayer() = 0;
    virtual bool changeDirection() = 0;
    virtual int addPenaltyCards() = 0;
    virtual bool changeColor() = 0;
    virtual ~ICard() = default;
};

class Card : public ICard
{
private:
    Type card_type_;
    Color card_color_;

public:
    // TODO Constructor
    Card(Type type, Color color) : card_type_{type}, card_color_{color} {};
    // TODO Destructor
    ~Card() = default;

    Color getColor() const override;
    Type getType() const override;
    // int getValue() override; // Undefined if isSpecial = true
    bool isSpecial() override; //! INFRACCION CAMBIADOS A const isSpecial, skips, cahngeColor, penaltyCards, changeDirection
    //                                  //! MOTIVO: declaro en playCard() una const ICard &card que necesariamente requiere de metodos const
    bool skipsPlayer() override;
    bool changeDirection() override;
    int addPenaltyCards() override;
    bool changeColor() override;
};

#endif // UNO_INTERFACES_ICARD_HPP