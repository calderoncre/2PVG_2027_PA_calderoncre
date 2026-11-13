#pragma once
#ifndef ESAT_VIEW_HPP
#define ESAT_VIEW_HPP
#include <cassert>
#include "esat/sprite.h"
#include "esat/window.h"
#include "blackjack/iview.hpp"
#include "blackjack/card.hpp"
using namespace blackjack;
namespace blackjack
{
    class EsatView : public blackjack::IView
    {
    public:
        EsatView() : selected_chip{-1}, selected_player_spot{-1}, puede_repartir{false}, turn{0}, action_Selected{Action::None}
        {
            setEsatText();
            LoadSprites();
            setSpotPositions();

            win_width = static_cast<float>(esat::WindowWidth());
            win_height = static_cast<float>(esat::WindowHeight());
            for (int i = 0; i < 4; ++i)
            {
                button_pos[i] = {
                    win_width - 650.0f + i * 150.0f,
                    win_height - 120.0f};
            }
        }
        ~EsatView()
        {
            ReleaseSprites();
        };
        int getSelectedChip() const;
        int getSelectedPlayerSpot() const;
        bool get_puede_repartir() const;
        int chipIndex_toValue(int index) const;
        int isBetInSpot(const int *bets) const;
        bool hasBetSelection() const;

        void printAPUESTAS(const int *bets) const;
        void printREPARTOS(const ITable &table) const;
        void printJUEGO(const ITable &table) const;
        void updateAPUESTAS();
        void updateJUEGO();
        Move getActionSelected();

        //! overrides
        virtual int chooseRules(const IRules *const *rules, int count) override;
        IPlayer *human() override;
        void update(const ITable &table) override;
        void draw(const ITable &table) override;
        bool busy() const override;
        bool quit() const override;
        void onEvent(const GameEvent &event, const ITable &table) override;

    private:
        void drawChipStacks(const int *bets) const;
        void drawChipStacks(const ITable &table) const;

        static const int n_chips_types = 5;
        esat::SpriteHandle card_sprites[kRankCount][kSuitCount]{};
        esat::SpriteHandle card_animations[4][4]{};
        esat::SpriteHandle card_animation_frames[4][4][8]{};
        esat::SpriteHandle button_sprites[4]{};
        esat::SpriteHandle chips_sprites[n_chips_types]{};
        esat::SpriteHandle backside_card_sprite = nullptr;
        esat::SpriteHandle background_card_sprite = nullptr;
        esat::SpriteHandle chip_stack_sprite = nullptr;
        esat::Vec2 spot_pos[7]{};
        esat::Vec2 button_pos[4]{};
        int selected_chip;        // -1 ninguna
        int selected_player_spot; // -1 ninguno
        bool puede_repartir;
        float win_width;
        float win_height;
        // juego
        int turn;
        Move action_Selected;

        void LoadSprites();
        void ReleaseSprites();

        void printSelectableRectangles(esat::Vec2 pos, float ancho, float height) const;
        void printColoredSquare(esat::Vec2 pos, int r, int g, int b) const;
        bool isMouseInsideRect(esat::Vec2 pos, float width, float height) const;
        void printAlphaLerpRect(esat::Vec2 pos, float width, float height) const;

        const char *value_to_char(blackjack::Rank valor) const;
        const char *suit_to_char(blackjack::Suit suit) const;
        void setEsatText();
        Move getInputReturnMove();

        esat::SpriteHandle getSpriteFromCard(Card card);
        void drawFloatingCard(esat::SpriteHandle sprite, float x, float y, float phase) const;
        void setSpotPositions();
    };
}

#endif