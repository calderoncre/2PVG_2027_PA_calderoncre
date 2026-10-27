#pragma once
#ifndef UNO_INTERFACES_IVIEW_HPP
#define UNO_INTERFACES_IVIEW_HPP

#include "BoolAction.hpp"
#include "Color.hpp"
#include "ICard.hpp"
#include "IDeck.hpp"
#include "IModel.hpp"
#include "esat/sprite.h"
#include "esat/draw.h"
#include <vector>
#include <string>

class IView
{
public:
    virtual ~IView() = default;
    virtual void print() const = 0;

    virtual void update() = 0;
    // // Tiene que tener opcion nula
    // virtual BoolAction pass() const = 0;
    virtual const ICard *cardSelected() const = 0;
    virtual bool drawWanted() const = 0;
    virtual Color colorSelected() const = 0;
    virtual BoolAction unoSaid() const = 0;
    virtual bool accuseUno() const = 0;
    virtual BoolAction wantChallenge() const = 0;

    virtual void clearSelections() = 0; // !!!! cambiar mi metodo a virtual
};

class UI : public IView
{
private:
    esat::SpriteHandle card_sprites[15][5];
    esat::SpriteHandle backside_card_sprite;
    const IModel &model_;
    const ICard *selected_card_;
    int index_selec_card;
    bool colorPalette_active;

    bool active_message_;
    std::string current_message_;
    double message_time_;

public:
    // TODO constructor
    UI(const IModel &model) : model_{model}, selected_card_{nullptr}, index_selec_card{-1}, active_message_{false}, current_message_{""}, message_time_{0.0f}, colorPalette_active{false}
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                card_sprites[i][j] = nullptr;
            }
        }
    };

    void print() const override;
    void update() override;
    // // Tiene que tener opcion nula
    // BoolAction pass() const override;
    const ICard *cardSelected() const override;
    void clearSelections() override;
    bool drawWanted() const override;
    Color colorSelected() const override;
    BoolAction unoSaid() const override;
    bool accuseUno() const override;
    BoolAction wantChallenge() const override;
    // void clearSelections() override;
    //! TODO MIS FUNCIONES
    void LoadSprites();
    void ReleaseSprites();
    esat::SpriteHandle getCardSprite(const Type type, const Color color) const;

    void printCard(float x, float y, const ICard &card) const;
    void printPlayerDeck(const IDeck &deck, esat::Vec2 pos) const;
    void printBoard(const Deck &mazo_pila_to_print, const Deck &mazo_descarte_to_print, const Model state) const;

    char *type_to_char(Type type) const;
    char *color_to_char(Color color) const;

    void printSelectableRectangles(esat::Vec2 pos, float ancho, float alto) const;
    bool isMouseInsiderRectangle(esat::Vec2 pos, float ancho, float alto) const;
    esat::Vec2 cardPosition(int index, esat::Vec2 origin) const;
    int get_index_selectedCard() const;

    void setEsatText() const;
    void printTextButtons() const;
    void printRivalPlayer() const;
    void activateMessage(std::string message);
    void printMessages();
    void activateColorPalette();
    void desactivateColorPalette();
    void printColoredSquare(esat::Vec2 pos, int r, int g, int b) const;
    void printButtons() const;
    void printCurrentColor(esat::Vec2 pos, Color color) const;

    ~UI() = default;
};
#endif // UNO_INTERFACES_IVIEW_HPP