#include "IModel.hpp"
#include "cassert"
const IDeck &Model::getPlayerHand(int id) const
{
    return players_decks_[id];
}
const IDeck &Model::getDiscardDeck() const
{
    return discard_deck_;
}
const IDeck &Model::getDrawDeck() const
{
    return draw_deck_;
}
int Model::currentPlayer() const
{
    return turno_;
}
int Model::playerCount() const
{
    return n_players_;
}
void Model::set_playerCount(int n_players)
{
    n_players_ = n_players;
}

void Model::reserve_memory_player_decks()
{
    players_decks_.resize(n_players_);
}
void Model::set_turn()
{
    turno_ = 0;
}
void Model::set_playable_color(Card first_card_copy)
{
    playable_color_ = first_card_copy.getColor();
}

void Model::set_draw_deck()
{
    draw_deck_.get_cards().clear();
    draw_deck_.get_cards().reserve(108);

    int n_copias = 2;
    int n_copias_comodin = 4;
    // 2 cartas de cada color menos comodines
    for (int i = 0; i < n_copias; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                Type type = static_cast<Type>(j);
                Color color = static_cast<Color>(k);
                if (!draw_deck_.cardIsPossible(type, color) || color == Color::Null || type == Type::Ct0)
                    continue;
                draw_deck_.get_cards().emplace_back(type, color);
            }
        }
    }
    // un 0 por color
    for (int i = 0; i < 5; i++)
    {
        Color color = static_cast<Color>(i);
        if (color == Color::Null)
            continue;
        draw_deck_.get_cards().emplace_back(Type::Ct0, color);
    }
    // 4 cambiar color 4 chupa4. total 8
    for (int i = 0; i < n_copias_comodin; i++)
    {
        draw_deck_.get_cards().emplace_back(Type::Ctmas4, Color::Null);
        draw_deck_.get_cards().emplace_back(Type::Cambiar_color, Color::Null);
    }
} // deberian ser constructores //! ----

void Model::build_player_deck(Deck &mazo_pila, int index)
{
    const int n_cards_for_players = 7;

    for (int j = 0; j < n_cards_for_players; j++)
    {
        Card carta = draw_deck_.extractLastCard();
        players_decks_[index].addCard(carta);
    }
}

Deck &Model::get_player_deck(int player)
{
    return players_decks_[player];
}
Deck &Model::get_draw_deck()
{
    return draw_deck_;
}
Deck &Model::get_discard_deck()
{
    return discard_deck_;
}
Color Model::get_playable_color() const
{
    return playable_color_;
}

Direction Model::getPlayDirection() const
{
    return current_direction_;
}
void Model::sumaTurno()
{
    assert(turno_ <= n_players_ && turno_ >= 0 && "turno fuera de rango");
    turno_++;
    if (turno_ >= n_players_)
        turno_ = 0;
}
void Model::restaTurno()
{
    assert(turno_ >= 0 && turno_ < n_players_ && "turno fuera de rango");
    turno_--;
    if (turno_ < 0)
        turno_ = n_players_ - 1;
}

void Model::calculate_turn() // caundo un jugador add_carta valida al mazo pila
{
    if (current_direction_ == Direction::Right)
        sumaTurno();
    else
        restaTurno();
}
bool Model::get_awaiting_color() const
{
    return awaiting_color;
}
bool Model::ProcessCard(ICard &player_card, Color selec_color, Color last_color, Type selec_type, Type last_type)
{
    int cartas_a_robar;
    if (!player_card.isSpecial())
    {
        if (selec_color != last_color && selec_type != last_type)
            return false;
        if (selec_color == last_color)
            return true;
        if (selec_type == last_type)
            return true;
    }
    else
    {
        // detecto comodines primero
        if (selec_type == Type::Cambiar_color)
        {
            awaiting_color = true;
            return true;
        }
        if (selec_type == Type::Ctmas4)
        {
            awaiting_color = true;
            cartas_a_robar = player_card.addPenaltyCards(); // ! todavia en el chupate4 el color lo elige el siguiente jugador
            robarCartas(cartas_a_robar);

            return true;
        }
        if (selec_color != last_color && selec_type != last_type)
            return false;
        if (selec_type == last_type || selec_color == last_color)
        {
            if (player_card.skipsPlayer())
            {
                return true;
                // efecto
            }
            if (player_card.changeColor())
            {
                // elegir color
                awaiting_color = true;
                return true;
                // efecto
            }
            if (player_card.changeDirection())
            {
                if (current_direction_ != Direction::Right)
                    current_direction_ = Direction::Right;
                else
                    current_direction_ = Direction::Left;
                return true;
            }
            if (selec_type == Type::Ctmas2)
            {
                cartas_a_robar = player_card.addPenaltyCards();
                robarCartas(cartas_a_robar);
                return true;
            }
        }
    }
    return false;
}
void Model::robarCartas(int cartas_a_robar)
{
    calculate_turn(); // para hacer que currentPlayer() ya sea el sigiente jugador
    for (int i = 0; i < cartas_a_robar; i++)
    {
        Card card = draw_deck_.extractLastCard();
        players_decks_[currentPlayer()].addCard(card);
    }
}

bool Model::playCard(int cardIndex, bool unoSaid)
{
    if (awaiting_color)
        return false;
    const int player = currentPlayer();
    std::vector<Card> &hand = players_decks_[player].get_cards();

    if (cardIndex < 0 || cardIndex >= static_cast<int>(hand.size()))
        return false;

    Card played = hand[cardIndex];
    const Card &last = discard_deck_.get_cards().back();
    if (!ProcessCard(played, played.getColor(), playable_color_, played.getType(), last.getType()))
        return false;
    // ! mover carta
    hand.erase(hand.begin() + cardIndex);
    discard_deck_.addCard(played);
    if (awaiting_color) // a partir de aqui el comodin estaría jugado
        return true;
    playable_color_ = played.getColor();
    if (played.skipsPlayer())
        calculate_turn();
    calculate_turn();
    return true;
}
void Model::drawCard()
{
    Card card = draw_deck_.extractLastCard();
    players_decks_[currentPlayer()].addCard(card);
    calculate_turn();
}
bool Model::chooseColor(Color color)
{
    if (!awaiting_color)
        return false;
    if (color != Color::Red && color != Color::Green &&
        color != Color::Blue && color != Color::Yellow)
        return false;
    playable_color_ = color;
    awaiting_color = false;
    calculate_turn();
    return true;
}

// CArta color Null
// return true;
