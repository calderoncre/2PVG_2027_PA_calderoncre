#pragma once
#ifndef UNO_INTERFACES_IMODEL_HPP
#define UNO_INTERFACES_IMODEL_HPP

#include "vector"
#include "Direction.hpp"
#include "IDeck.hpp"
#include "IPlayer.hpp"

class IModel
{
    //! A implementar
public:
    virtual int playerCount() const = 0;
    virtual int currentPlayer() const = 0;
    virtual const IDeck &getDrawDeck() const = 0;
    virtual const IDeck &getDiscardDeck() const = 0;
    virtual const IDeck &getPlayerHand(int id) const = 0;
    virtual Direction getPlayDirection() const = 0;
    virtual Color get_playable_color() const = 0; //! INFRACCION
    // virtual bool isGameEnded() const = 0;
    // virtual int winner() const = 0; // -1 == None
    virtual void drawCard() = 0;
    virtual bool playCard(int cardIndex, bool unoSaid) = 0;
    // virtual IModel *clone() const = 0;

    virtual ~IModel() = default;

    //! mis funciones
};
class Model : public IModel // !! el game_state básicamente
{
private:
    const int total_cards_;
    int n_players_;
    int turno_;
    bool awaiting_color;
    bool puede_jugar_;

    std::vector<Deck> players_decks_;
    Deck draw_deck_;
    Deck discard_deck_;
    Color playable_color_;
    Direction current_direction_;

public:
    Model() : total_cards_{108}, current_direction_{Direction::Right}, playable_color_{Color::Red}, puede_jugar_{true}, awaiting_color{false} {};

    int playerCount() const override;
    int currentPlayer() const override;
    const IDeck &getDrawDeck() const override;
    const IDeck &getDiscardDeck() const override;
    const IDeck &getPlayerHand(int id) const override;
    Color get_playable_color() const override;

    Direction getPlayDirection() const override;
    // bool isGameEnded() const override;
    // int winner() const override;
    void drawCard() override;
    bool playCard(int cardIndex, bool unoSaid) override;
    // IModel *clone() const override;

    //! mis funciones
    void set_playerCount(int n_players);
    void reserve_memory_player_decks();
    void set_turn();
    void set_playable_color(Card first_card_copy);
    void set_draw_deck(); // deberian ser constructores //! ----
    void build_player_deck(Deck &mazo_pila, int index);
    Deck &get_player_deck(int player);
    Deck &Model::get_draw_deck();
    Deck &Model::get_discard_deck();
    bool Model::ProcessCard(ICard &player_card, Color selec_color, Color last_color, Type selec_type, Type last_type);
    bool get_awaiting_color() const;
    void sumaTurno();
    void restaTurno();
    void calculate_turn();
    bool chooseColor(Color color);
    void robarCartas(int cartas_a_robar);
};

#endif // UNO_INTERFACES_IMODEL_HPP