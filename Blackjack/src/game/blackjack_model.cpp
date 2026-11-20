#include <vector>
#include "blackjack_model.hpp"
#include "cassert"
namespace blackjack
{
    // pasamos la vista al controlador desde iOberver
    //  los eventos los emites desde la view
    //  el futuro player escuchará eventos

    int BlackjackModel::getTurn() const
    {
        return turno;
    }
    int BlackjackModel::getnActivePlayers() const
    {
        return n_active_players;
    }
    bool BlackjackModel::substractChips(int quantity, int seat)
    {
        if (seats[seat].chips >= quantity && seats[seat].chips > 0)
        {
            seats[seat].chips -= quantity;
            return true;
        }
        else
            return false;
    }

    void BlackjackModel::initDealerCards(int n_barajas)
    {
        for (int k = 0; k < n_barajas; k++)
        {
            for (int i = 0; i < kRankCount; i++)
            {
                for (int j = 0; j < kSuitCount; j++)
                {
                    Rank rank = static_cast<Rank>(i + 1);
                    Suit suit = static_cast<Suit>(j);
                    Card card(rank, suit);
                    pile_deck.emplace_back(rank, suit);
                }
            }
        }

        printf("cartas añadidas: %d\n", static_cast<int>(pile_deck.size()));
    }

    HandState BlackjackModel::initPlayerHand()
    {
        HandState hand;
        // hand.stake = se lo pasa esatView
        hand.bet = hand.stake;
        hand.result = 0;
        hand.from_split = false;
        hand.split_aces = false;
        hand.doubled = false;
        hand.twisted = false;
        hand.surrendered = false;
        hand.rescued = false;
        hand.finished = false;
        return hand;
    }

    void BlackjackModel::initTableSettings()
    {
        table_settings.max_bet = maxBet();
        table_settings.min_bet = minBet();
        // ojo con esto  antes estaba a 0 y 0
        table_settings.seat_count = kMaxSeats;
        table_settings.starting_chips = 1000;
        table_settings.seed = 99; // posteriormente rand();
    }

    void BlackjackModel::addCardtoPlayer(Card card, int seat, int hand)
    {
        seats[seat].hand[hand].cards.push_back(card);
    }

    void BlackjackModel::addCardtoDealer(Card card, int vuelta)
    {
        if (vuelta > 2)
            return;
        if (model_rules.openingDealerCard(vuelta) == OpeningCard::None)
            return;
        if (model_rules.openingDealerCard(vuelta) == OpeningCard::FaceDown)
            up_dcards.push_back(card);
        else if (model_rules.openingDealerCard(vuelta) == OpeningCard::FaceUp)
            hidden_dcards.push_back(card);
    }

    bool BlackjackModel::getMarcarCorte() const
    {
        return marcar_corte;
    }

    Card BlackjackModel::extractLastCard(std::vector<Card> &deck)
    {
        if (&deck == &pile_deck)
            onEmptyPileDeck();

        assert(!deck.empty());
        Card card = deck.back();
        deck.pop_back();
        return card;
    }

    Card BlackjackModel::extractCard(int index) // ! quitar no se usa de momento
    {
        assert(index >= 0 && index < static_cast<int>(pile_deck.size()));
        Card card = pile_deck[index];
        pile_deck.erase(pile_deck.begin() + index);
        return card;
    }

    //! ITable
    const char *BlackjackModel::rulesName() const
    {
        return "";
    }

    int BlackjackModel::minBet() const
    {
        return 5;
    }

    int BlackjackModel::maxBet() const
    {
        return 100;
    }

    int BlackjackModel::cardsLeftInShoe() const
    {
        return static_cast<int>(pile_deck.size());
    }

    int BlackjackModel::seatCount() const
    {
        return kMaxSeats;
    }

    void BlackjackModel::set_active_players()
    {
        n_active_players = 0;
        int index = -1;
        for (int seat = 0; seat < seatCount(); ++seat)
        {
            if (!seats[seat].hand.empty())
            {
                ++index;
                current_playing[index] = seat;
                printf("current_player index %d:", current_playing[index]);
                ++n_active_players;
            }
        }
    }

    int BlackjackModel::chips(int seat) const
    {
        return seats[seat].chips;
    }

    int BlackjackModel::handCount(int seat) const
    {
        return static_cast<int>(seats[seat].hand.size());
    }

    HandState BlackjackModel::hand(int seat, int hand) const
    {
        const HandData &data = seats[seat].hand[hand];
        HandState result = data.state;
        result.cards = std::span<const Card>(data.cards); // cargo mi data en su span para poder consultar
        return result;
    }

    int BlackjackModel::dealerHiddenCount() const
    {
        return static_cast<int>(hidden_dcards.size());
    }

    int BlackjackModel::insuranceResult(int seat) const
    {
        return 0;
    }

    std::span<const Card> BlackjackModel::dealerUpCards() const
    {
        return std::span<const Card>(up_dcards);
    }

    BuyRange BlackjackModel::buyRange(int seat, int hand) const
    {
        return {1, 0};
    }

    //! IModel
    void BlackjackModel::startRound(const int *bets)
    {
        addAllCardsToDiscardDeck_atRoundEnded(); // antes del clear
        current_vuelta = 0;
        current_spot = 0;
        turno = 0;
        hand_count = 0;
        hidden_dcards.clear();
        up_dcards.clear();
        if (comprobarCorte())
            marcar_corte = true;
        for (int i = 0; i < kMaxSeats; i++)
        {
            seats[i].hand.clear();

            if (bets[i] < minBet() || bets[i] > maxBet())
                continue;
            if (!substractChips(bets[i], i))
                continue;

            HandData tmp_hand{};
            tmp_hand.state.stake = bets[i];
            tmp_hand.state.bet = bets[i];
            seats[i].hand.push_back(tmp_hand);
            printf("sitio: %d, chips: %d\n", i, seats[i].chips);
        }
        if (need_to_shuffle)
        {
            std::shuffle(pile_deck.begin(), pile_deck.end(), generator);
            need_to_shuffle = false;
        }
        if (marcar_corte)
        {
            while (!discard_deck.empty())
            {
                Card card_to_pile = extractLastCard(discard_deck);
                pile_deck.push_back(card_to_pile);
            }
            std::shuffle(pile_deck.begin(), pile_deck.end(), generator);
            marcar_corte = false;
        }
        set_active_players(); // setear jugadores activos en esta ronda
    }
    void BlackjackModel::onEmptyPileDeck()
    {
        if (!pile_deck.empty())
            return;
        while (!discard_deck.empty())
        {
            pile_deck.push_back(discard_deck.back());
            discard_deck.pop_back();
        }
        std::shuffle(pile_deck.begin(), pile_deck.end(), generator);
        marcar_corte = true; // para recordar que al empezar la siguiente ronda toca reunir y barajar todo, aunque hayan mas de 52 cards
    }
    bool BlackjackModel::dealNext()
    {
        //! Falta Consultar openingDealerCard(current_vuelta) para saber si el dealer recibe carta y si va visible u oculta.
        //! Registrar los eventos de las cartas repartidas

        while (current_vuelta < 2)
        {
            if (current_spot < kMaxSeats)
            {
                const int seat = current_spot;
                ++current_spot; // para el siguiente ya lo preparo
                if (seats[seat].hand.empty())
                    continue;

                // extraer carta
                Card card = extractLastCard(pile_deck);
                addCardtoPlayer(card, seat, 0);
                // repartir si corresponde
                return true;
            }
            // repartir al dealer
            Card card = extractLastCard(pile_deck);
            addCardtoDealer(card, current_vuelta);
            // preparar la siguiente vuelta
            current_spot = 0;
            ++current_vuelta;
            return true;
        }
        return false;
    }
    void BlackjackModel::avanzarTurno()
    {
        assert(turno >= 0 && turno <= 7 && "turno fuera de rango");
        hand_count = 0;
        ++turno;
    }
    bool BlackjackModel::avanzarMano(int seat)
    {
        // si existe mano posible para avanzar
        if (hand_count + 1 < static_cast<int>(seats[seat].hand.size()))
        {
            ++hand_count;
            return true;
        }
        return false;
    }

    bool BlackjackModel::insuranceOffered(int seat) const
    {
        // si cae un as ofrecemos insurance
        return true;
    }

    void BlackjackModel::takeInsurance(int seat)
    {
        // int takeInsurance
    }

    PeekResult BlackjackModel::peek()
    {
        return PeekResult::NoPeek;
    }

    void BlackjackModel::startHand(int seat, int hand)
    {
        // 1 Acceder al jugador y a su mano
        const HandState &mano = this->hand(seat, hand);

        // 2 Comprobar si la mano procede de un split
        //    y necesita su segunda carta
        if (mano.from_split && static_cast<int>(mano.cards.size()) < 2)
        {
            // 3 si la necesita
            //    - Extraer una carta de pile_deck
            //    - Añadirla a la mano
            Card card = extractLastCard(pile_deck);
            addCardtoPlayer(card, seat, hand);
        }
        // 4 Comprobar si la mano debe finalizar
        //    según las reglas
        HandState actualizada = this->hand(seat, hand);
        seats[seat].hand[hand].state.finished =
            model_rules.endsTurn(actualizada);
    }

    bool BlackjackModel::apply(int seat, int hand, const Move &move)
    {
        if (!isAllowed(seat, hand, move.action))
            return false;
        switch (move.action)
        {
        case Action::None:
            printf("trying to apply() invalid action\n");
            return false;

        case Action::Hit:
        {
            Card card_to_deal = extractLastCard(pile_deck);
            addCardtoPlayer(card_to_deal, seat, hand);
            HandState tmp_hand = this->hand(seat, hand);
            if (model_rules.endsTurn(tmp_hand))
                seats[seat].hand[hand].state.finished = true;
            return true;
        }

        case Action::Stand:
            seats[seat].hand[hand].state.finished = true;
            return true;

        case Action::Double:
        {
            const int apuesta = seats[seat].hand[hand].state.bet;
            if (!substractChips(apuesta, seat))
                return false;

            seats[seat].hand[hand].state.bet *= 2;
            seats[seat].hand[hand].state.doubled = true;

            Card card = extractLastCard(pile_deck);
            addCardtoPlayer(card, seat, hand);
            HandState tmp_hand = this->hand(seat, hand);
            if (model_rules.endsTurn(tmp_hand))
                seats[seat].hand[hand].state.finished = true;
            return true;
        }

        case Action::Split:
        {
            // cobrar apuesta
            const int apuesta = seats[seat].hand[hand].state.bet;
            if (!substractChips(apuesta, seat))
                return false;
            const bool son_ases = seats[seat].hand[hand].cards[0].rank == Rank::Ace;

            // la magia
            HandData new_hand = seats[seat].hand[hand];
            new_hand.cards.erase(new_hand.cards.begin());
            seats[seat].hand[hand].cards.pop_back();
            // marcar como divididas
            // from split , from aces activar
            seats[seat].hand[hand].state.from_split = true;
            seats[seat].hand[hand].state.split_aces = son_ases;
            seats[seat].hand[hand].state.finished = false;

            new_hand.state.from_split = true;
            new_hand.state.split_aces = son_ases;
            new_hand.state.finished = false;
            new_hand.state.stake = apuesta;
            new_hand.state.bet = apuesta;

            seats[seat].hand.push_back(new_hand);

            Card card = extractLastCard(pile_deck);
            addCardtoPlayer(card, seat, hand);
            return true;
        }

        default:
            break;
        }
        return false;
    }

    bool BlackjackModel::isAllowed(int seat, int hand, Action action) const
    {
        const HandState &hand_ = this->hand(seat, hand);
        if (hand_.finished || action == Action::None)
            return false;
        return (model_rules.isAllowed(hand_, chips(seat), handCount(seat), action));
    }
    void BlackjackModel::reiniciarHandCount()
    {
        hand_count = 0;
    }

    SeatState BlackjackModel::getSeat(int seat) const
    {
        return seats[seat];
    }

    bool BlackjackModel::playDealer()
    {
        // turn up card/s
        if (!hidden_dcards.empty())
        {
            // paso al carta de hidden cards a up cards
            Card card = hidden_dcards.back();
            hidden_dcards.pop_back();
            up_dcards.push_back(card);
            return true;
        }
        // draw card mientras pueda
        if (model_rules.dealerDraws(up_dcards, hasHandsDepedingOnDealer()))
        {
            up_dcards.push_back(extractLastCard(pile_deck));
            return true;
        }
        return false;
    }

    bool BlackjackModel::hasHandsDepedingOnDealer()
    {
        // por cada asiento
        for (int i = 0; i < seatCount(); i++)
        {
            // por cada mano de cada asiento
            for (int j = 0; j < handCount(i); j++)
            {
                HandState mano = this->hand(i, j);
                if (!model_rules.decidedBeforeDealer(mano)) // de momento si no es > 21
                    return true;
            }
        }
        return false;
    }

    void BlackjackModel::settle() // decidir que jugdores han ganado, perdido o empatado con el crupier
    {
        for (int seat = 0; seat < seatCount(); seat++)
        {
            for (int i = 0; i < handCount(seat); i++)
            {
                // obtener la mano del jugador
                HandState mano = this->hand(seat, i);

                // consultar las reglas para determinar
                //    el resultado de la mano
                // calcular ganancias y pérdidas
                int pago = model_rules.payout(mano, up_dcards);

                // guardar el resultado
                //    seats[seat].hand[i].state.result
                seats[seat].hand[i].state.result = pago - mano.bet;
                // actualizar las fichas del jugador
                seats[seat].chips += pago;
            }
            // ! liquidar el seguro de este jugador (de momento no hay seguro)
        }
    }

    void BlackjackModel::addAllCardsToDiscardDeck_atRoundEnded()
    {
        // cartas de lo jugadores
        for (int seat = 0; seat < seatCount(); seat++)
        {
            for (int hand = 0; hand < handCount(seat); hand++)
            {
                HandState mano = this->hand(seat, hand);
                for (const Card &card : mano.cards)
                {
                    if (!mano.cards.empty())
                        discard_deck.push_back(card);
                }
            }
        }
        // cartas del dealer
        for (const Card &card : dealerUpCards())
        {
            if (!dealerUpCards().empty())
                discard_deck.push_back(card);
        }
        for (const Card &card : dealerHiddenCards())
        {
            if (!dealerHiddenCards().empty())
                discard_deck.push_back(card);
        }
    }
    bool BlackjackModel::comprobarCorte() const
    {
        return static_cast<int>(pile_deck.size()) <= 52;
    }

    std::span<const Card> BlackjackModel::dealerHiddenCards() const
    {
        return hidden_dcards;
    }

    void BlackjackModel::setRondaLiquidada(bool estado)
    {
        ronda_liquidada = estado;
    }

    bool BlackjackModel::getRondaLiquidada() const
    {
        return ronda_liquidada;
    }

    bool BlackjackModel::popEvent(ModelEvent *event)
    {
        return true;
    }

    int BlackjackModel::currentSeat() const
    {
        if (turno < 0 || turno >= n_active_players)
            return -1;
        return current_playing[turno];
    }

    int BlackjackModel::currentHand() const
    {
        return hand_count;
    }

}