#pragma once
#ifndef BLACKJACK_MODEL
#define BLACKJACK_MODEL 1
#include <vector>
#include <algorithm>
#include <random>
#include "blackjack/imodel.hpp"
#include "game_rules.hpp"
namespace blackjack
{
    struct HandData
    {
        std::vector<Card> cards;
        HandState state{};
    };
    struct SeatState
    {
        int chips;
        std::vector<HandData> hand;
    };

    class BlackjackModel : public IModel
    {
    private:
        SeatState seats[kMaxSeats];
        std::vector<Card> pile_deck;
        // std::vector<Card> dealer_cards;
        std::vector<Card> hidden_dcards; // 1
        std::vector<Card> up_dcards;     // 1
        std::vector<Card> discard_deck;  // cards discarded every time a round ends
        TableSettings table_settings;
        const IRules &model_rules; // consulta de reglas
        std::mt19937 generator;

        int turno;
        bool players_waiting;
        int current_playing[kMaxSeats];
        int hand_count;
        int n_active_players;
        // reparto
        int current_vuelta;
        int current_spot;
        bool ronda_liquidada;
        bool need_to_shuffle;
        bool marcar_corte;

        void initTableSettings();
        void initDealerCards(int n_barajas);
        HandState initPlayerHand();
        void addCardtoPlayer(Card card, int seat, int hand);
        void addCardtoDealer(Card card, int vuelta);
        Card extractLastCard(std::vector<Card> &deck);
        Card extractCard(int index);
        void set_active_players();
        void addAllCardsToDiscardDeck_atRoundEnded();
        void onEmptyPileDeck();

    public:
        bool comprobarCorte() const;
        void setRondaLiquidada(bool estado);
        bool getRondaLiquidada() const;
        bool substractChips(int quantity, int seat);
        SeatState getSeat(int seat) const;
        void avanzarTurno();
        int getTurn() const;
        int getnActivePlayers() const;
        bool avanzarMano(int seat);
        void reiniciarHandCount();
        bool hasHandsDepedingOnDealer();
        bool getMarcarCorte() const;

        BlackjackModel(const IRules &rules) : model_rules(rules), turno{0}, hand_count{0}, players_waiting{true}, ronda_liquidada{false}, need_to_shuffle{true}, marcar_corte{false}
        {
            initTableSettings();
            for (int i = 0; i < kMaxSeats; i++)
                seats[i].chips = table_settings.starting_chips;

            generator.seed(table_settings.seed);
            initDealerCards(4);
        }
        //! ITable
        const char *rulesName() const override;
        int minBet() const override;
        int maxBet() const override;
        int cardsLeftInShoe() const override;
        int seatCount() const override;
        int chips(int seat) const override;
        int handCount(int seat) const override;
        HandState hand(int seat, int hand) const override;
        int dealerHiddenCount() const override;
        int insuranceResult(int seat) const override;
        bool isAllowed(int seat, int hand, Action action) const override;
        BuyRange buyRange(int seat, int hand) const override;
        std::span<const Card> dealerUpCards() const override;
        virtual int currentSeat() const override;
        virtual int currentHand() const override;

        //! IModel
        void startRound(const int *bets) override; //-- 1st
        bool dealNext() override;
        bool insuranceOffered(int seat) const override;
        void takeInsurance(int seat) override;
        PeekResult peek() override;
        void startHand(int seat, int hand) override; // -- 2st
        bool apply(int seat, int hand, const Move &move) override;
        bool playDealer() override;
        void settle() override;
        bool popEvent(ModelEvent *event) override;
        std::span<const Card> dealerHiddenCards() const;
    };

}
#endif