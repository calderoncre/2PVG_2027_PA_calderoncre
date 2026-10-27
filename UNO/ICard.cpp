#include "ICard.hpp"
Color Card::getColor() const
{
    return card_color_;
}
Type Card::getType() const
{
    return card_type_;
}
bool Card::isSpecial()
{
    if (card_type_ == Type::Cambiar_color || card_type_ == Type::Cambiar_direccion || card_type_ == Type::Ctmas2 || card_type_ == Type::Ctmas4 || card_type_ == Type::Prohibido)
        return true;
    return false;
}
bool Card::changeColor()
{
    if (card_type_ == Type::Cambiar_color)
        return true;
    return false;
}
bool Card::skipsPlayer()
{
    if (card_type_ == Type::Prohibido)
        return true;
    return false;
}
bool Card::changeDirection()
{
    if (card_type_ == Type::Cambiar_direccion)
        return true;
    return false;
}
int Card::addPenaltyCards()
{
    if (card_type_ == Type::Ctmas2)
        return 2;
    if (card_type_ == Type::Ctmas4)
        return 4;
    return 0;
}
