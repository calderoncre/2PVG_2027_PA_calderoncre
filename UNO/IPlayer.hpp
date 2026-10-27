#pragma once
#ifndef UNO_INTERFACES_IPLAYER_HPP
#define UNO_INTERFACES_IPLAYER_HPP

#include "BoolAction.hpp"
#include "ICard.hpp"
#include "IDeck.hpp"

class IPlayer
{
    //! A implementar
public:
    virtual ~IPlayer() = default;
    virtual BoolAction pass() const = 0;
    virtual ICard *cardSelected() const = 0;
    virtual bool drawWanted() const = 0;
    virtual Color colorSelected() const = 0;
    virtual BoolAction unoSaid() const = 0;
    virtual bool accuseUno() const = 0;
    virtual BoolAction wantChallenge() const = 0;
    //! Mis movidas
};
class HumanPlayer : public IPlayer
{
private:
    // Deck mazo_jugador_;
public:
    // Deck& getMazo_jugador()
    // {
    //     return mazo_jugador_;
    // };
    HumanPlayer() {

    };
};

#endif // UNO_INTERFACES_IPLAYER_HPP