
#include <string>
#include "esatView.hpp"
#include <cmath>
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/time.h"
#include "cassert"
using namespace blackjack;
namespace blackjack
{
    int EsatView::getSelectedChip() const
    {
        return selected_chip;
    }

    int EsatView::getSelectedPlayerSpot() const
    {
        return selected_player_spot;
    }

    bool EsatView::get_puede_repartir() const
    {
        return puede_repartir;
    }
    Move EsatView::getActionSelected()
    {
        return action_Selected;
    }
    void EsatView::setSpotPositions()
    {
        float win_width = static_cast<float>(esat::WindowWidth());
        float win_height = static_cast<float>(esat::WindowHeight());
        esat::Vec2 tmp[] =
            {
                {win_width / 2 - 300, win_height - 320}, // O || O O
                {100, win_height - 360},
                {100, 200},                          // | O O
                {win_width / 2 - 100, 60},           // O | O
                {win_width - 300, 200},              // O O |
                {win_width - 300, win_height - 360}, // || O O O
                {win_width / 2 + 100, win_height - 320}};
        for (int i = 0; i < 7; i++)
            spot_pos[i] = tmp[i];
    }
    void EsatView::updateJUEGO()
    {
        action_Selected = getInputReturnMove();
    }
    Move EsatView::getInputReturnMove()
    {
        //! Detectar Botones
        const bool click = esat::MouseButtonDown(0);
        Move current_move;
        current_move.amount = 1;
        current_move.action = Action::None;
        // TODO button split
        if (isMouseInsideRect({win_width - 600, win_height - 100}, 125, 45) && click)
        {
            current_move.action = Action::Split;
            printf("boton split!\n");
            return current_move;
        }
        // TODO buttons double
        if (isMouseInsideRect({win_width - 450, win_height - 100}, 125, 45) && click)
        {
            current_move.action = Action::Double;
            printf("boton double!\n");
            return current_move;
        }
        // TODO buttons hit
        if (isMouseInsideRect({win_width - 300, win_height - 100}, 125, 45) && click)
        {
            current_move.action = Action::Hit;
            printf("boton hit!\n");
            return current_move;
        }
        // TODO buttons stand
        if (isMouseInsideRect({win_width - 150, win_height - 100}, 125, 45) && click)
        {
            current_move.action = Action::Stand;
            printf("boton stand!\n");
            return current_move;
        }
        return current_move;
    }

    void EsatView::updateAPUESTAS()
    {
        //! TODO SELECTABLE RECTs CHIPS
        // buttons chips
        const bool click = esat::MouseButtonDown(0);
        if (click && selected_chip != -1)
            selected_player_spot = -1;
        for (int i = 0; i < n_chips_types; ++i)
        {
            esat::Vec2 pos{
                win_width / 2.0f - 380.0f + i * 80.0f + 50.0f,
                win_height - 100.0f};

            printSelectableRectangles(pos, 70.0f, 70.0f);

            if (click && isMouseInsideRect(pos, 70.0f, 70.0f))
            {
                selected_chip = i;
                selected_player_spot = -1;
                printf("chip: %d\n", selected_chip);
            }
        }
        // repartir button
        printColoredSquare({win_width - 600, win_height - 100}, 220, 0, 0);
        puede_repartir = false;
        if (isMouseInsideRect({win_width - 600, win_height - 100}, 120, 45) && click)
        {
            selected_chip = -1;
            selected_player_spot = -1;
            puede_repartir = true;
            printf("Boton pulsado y puede repartir\n");
        }

        // TODO players spots in table
        const int n_positions = 7;
        const esat::Vec2 *positions = spot_pos;
        for (int i = 0; i < n_positions; i++)
        {
            if (selected_chip != -1)
                printAlphaLerpRect(positions[i], 200, 200);

            if (click && isMouseInsideRect(positions[i], 200, 200))
            {
                selected_player_spot = i;
            }
            /* code */
        }
    }

    void EsatView::printAPUESTAS(const int *bets) const
    {
        float x = esat::WindowWidth() / 2 - 64;
        float y = esat::WindowHeight() / 2 - 120;
        esat::DrawSprite(background_card_sprite, 0, 0);
        drawFloatingCard(backside_card_sprite, x, y, x * 0.03f + y * 0.02f);

        for (int i = 0; i < n_chips_types; i++)
        {
            if (chips_sprites[i] != nullptr)
            {
                esat::DrawSprite(chips_sprites[i], (esat::WindowWidth() / 2 - 380) + (i * 80) + 50, esat::WindowHeight() - 100); // luego puntero si no pierdo la referencia
            }
        }

        drawChipStacks(bets);
    }

    void EsatView::printREPARTOS(const ITable &table) const
    {
        float x = esat::WindowWidth() / 2 - 64;
        float y = esat::WindowHeight() / 2 - 120;
        esat::DrawSprite(background_card_sprite, 0, 0);
        drawFloatingCard(backside_card_sprite, x, y, x * 0.03f + y * 0.02f);
        drawChipStacks(table);
    }
    void EsatView::printJUEGO(const ITable &table) const
    {
        float x = esat::WindowWidth() / 2 - 64;
        float y = esat::WindowHeight() / 2 - 120;
        esat::DrawSprite(background_card_sprite, 0, 0);
        drawFloatingCard(backside_card_sprite, x, y, x * 0.03f + y * 0.02f);
        drawChipStacks(table);

        //! Botones y texto
        esat::DrawSetTextSize(25.0f);
        esat::DrawSprite(button_sprites[0], win_width - 650, win_height - 120);
        esat::DrawSprite(button_sprites[1], win_width - 500, win_height - 120);
        esat::DrawSprite(button_sprites[2], win_width - 350, win_height - 120);
        esat::DrawSprite(button_sprites[3], win_width - 200, win_height - 120);
        for (int i = 0; i < 4; i++)
        {
            drawFloatingCard(button_sprites[i], button_pos[i].x, button_pos[i].y, button_pos[i].x * 0.2f + button_pos[i].y * 0.3f);
        }
        //! Imprimir suma total de cartas de cada jugador
    }

    void EsatView::printAlphaLerpRect(esat::Vec2 pos, float width, float height) const
    {
        float points[8]{
            pos.x, pos.y,
            pos.x + width, pos.y,
            pos.x + width, pos.y + height,
            pos.x, pos.y + height};

        const double segundos = esat::Time() / 1000.0;
        const double ciclo = segundos * 2.0 * 3.141592653589793 / 3.0;

        const double intensidad = (1.0 - std::cos(ciclo)) / 2.0;
        const unsigned char alfa = static_cast<unsigned char>(intensidad * 100.0);

        esat::DrawSetFillColor(255, 255, 255, alfa);
        esat::DrawSetStrokeColor(255, 255, 255, alfa);
        esat::DrawSolidPath(points, 4);
    }

    int EsatView::chipIndex_toValue(int index) const
    {
        switch (index)
        {
        case 0:
            return 1;
            break;
        case 1:
            return 5;
            break;
        case 2:
            return 10;
            break;
        case 3:
            return 20;
            break;
        case 4:
            return 50;
            break;
        default:
            return 0;
            break;
        }
    }

    void EsatView::printColoredSquare(esat::Vec2 pos, int r, int g, int b) const
    {
        int width = 120;
        int height = 45;
        float points[8]{
            pos.x, pos.y,
            pos.x + width, pos.y,
            pos.x + width, pos.y + height,
            pos.x, pos.y + height};
        esat::DrawSetFillColor(r, g, b, 255);
        esat::DrawSolidPath(points, 4);
    }

    void EsatView::printSelectableRectangles(esat::Vec2 pos, float width, float height) const
    {
        if (!isMouseInsideRect(pos, width, height))
            return;

        float points[8] = {
            pos.x, pos.y,
            pos.x + width, pos.y,
            pos.x + width, pos.y + height,
            pos.x, pos.y + height};
        esat::DrawSetFillColor(255, 255, 255, 50);
        esat::DrawSetStrokeColor(255, 255, 255, 50);
        esat::DrawSolidPath(points, 4);
        esat::DrawSetFillColor(255, 255, 255, 255);
    }

    bool EsatView::isMouseInsideRect(esat::Vec2 pos, float width, float height) const
    {
        float mouseX = esat::MousePositionX();
        float mouseY = esat::MousePositionY();
        return mouseX >= pos.x && mouseX <= pos.x + width && mouseY >= pos.y && mouseY <= pos.y + height;
    }

    bool EsatView::hasBetSelection() const
    {
        return selected_chip != -1 && selected_player_spot != -1;
    }

    void EsatView::drawChipStacks(const int *bets) const
    {
        const esat::Vec2 positions[] = {
            {win_width / 2 - 300, win_height - 320}, // O || O O
            {100, win_height - 360},
            {100, 200},                          // | O O
            {win_width / 2 - 100, 60},           // O | O
            {win_width - 300, 200},              // O O |
            {win_width - 300, win_height - 360}, // || O O O
            {win_width / 2 + 100, win_height - 320}};
        // imprimiir sprite si hay apuesta
        for (int i = 0; i < 7; ++i)
        {
            if (bets[i] <= 0)
                continue;
            esat::DrawSprite(chip_stack_sprite, positions[i].x, positions[i].y - 40);
            const std::string text = std::to_string(bets[i]);
            esat::DrawSetFillColor(255, 255, 255, 255);
            esat::DrawText(positions[i].x + 45, positions[i].y - 13, text.c_str());
        }
    }
    void EsatView::drawChipStacks(const ITable &table) const
    {
        for (int seat = 0; seat < table.seatCount(); ++seat)
        {
            for (int h = 0; h < table.handCount(seat); ++h)
            {
                const HandState hand = table.hand(seat, h);

                if (hand.bet <= 0)
                    continue;

                const float x = spot_pos[seat].x;
                const float y = spot_pos[seat].y + h * 80.0f;
                esat::DrawSprite(chip_stack_sprite, x, y - 40);

                const std::string text = std::to_string(hand.bet);
                esat::DrawSetFillColor(255, 255, 255, 255);
                esat::DrawText(x + 45, y - 13, text.c_str());
            }
        }
    }

    void EsatView::setEsatText()
    {
        esat::DrawSetTextSize(20.0f);
        esat::DrawSetTextFont("src/fonts/mi_fuente2.ttf");
    }

    void EsatView::LoadSprites()
    {
        //! SPRITES FIJOS
        std::string cadena = "";
        for (int i = 0; i < kRankCount; i++)
        {
            for (int j = 0; j < kSuitCount; j++)
            {
                // le estoy diciendo al compilador: interpreta este entero como uno de los valores de mi enum
                blackjack::Rank value = static_cast<blackjack::Rank>(i + 1);
                blackjack::Suit suit = static_cast<blackjack::Suit>(j);
                cadena.append("src/game/sprites/nuevo/PixelPlebes_Complete/PixelPlebes_Complete/Plebes_Update002-ANIMS/V1/1X/");
                // cadena.append(palo_to_char(getpalo(i))); //! no podia hacer esto claro
                cadena.append(value_to_char(value));
                cadena.append("_");
                cadena.append(suit_to_char(suit));
                cadena.append(".png");
                printf("%s \n", cadena.c_str());
                card_sprites[i][j] = esat::SpriteFromFile(cadena.c_str());
                assert(card_sprites[i][j] != nullptr && "ERROR loading card sprites");

                cadena.clear();
            }
        }
        cadena.clear();
        const int valores[n_chips_types] = {1, 5, 10, 20, 50};
        for (int i = 0; i < n_chips_types; ++i)
        {
            std::string ruta = "src/game/sprites/ficha_" + std::to_string(valores[i]) + ".png";
            chips_sprites[i] = esat::SpriteFromFile(ruta.c_str());
            assert(chips_sprites[i] != nullptr && "ERROR loading chips sprites");
        }

        backside_card_sprite = esat::SpriteFromFile("src/game/sprites/nuevo/PixelPlebes_Complete/PixelPlebes_Complete/Plebes_Update002-ANIMS/V1/1X/reverso.png");
        background_card_sprite = esat::SpriteFromFile("src/game/sprites/background_pixel_art.png");
        chip_stack_sprite = esat::SpriteFromFile("src/game/sprites/chip_stack.png");

        // botones
        button_sprites[0] = esat::SpriteFromFile("src/game/sprites/button_Split.png");
        button_sprites[1] = esat::SpriteFromFile("src/game/sprites/button_double.png");
        button_sprites[2] = esat::SpriteFromFile("src/game/sprites/button_hit.png");
        button_sprites[3] = esat::SpriteFromFile("src/game/sprites/button_stand.png");

        //! SPRITE SHEETS A, K, Q, J

        const char *rank_folders[] = {"Ace", "King", "Queen", "Jack"};
        const char *rank_names[] = {"as", "rey", "reina", "jota"};
        const char *suit_folders[] = {"Club", "Diamond", "Heart", "Spade"};
        const char *suit_names[] = {"treboles", "diamantes", "corazones", "picas"};

        const std::string base =
            "src/game/sprites/nuevo/PixelPlebes_Complete/"
            "PixelPlebes_Complete/Plebes_Update002-ANIMS/V1/Animations/";

        for (int rank = 0; rank < 4; ++rank)
        {
            for (int suit = 0; suit < 4; ++suit)
            {
                std::string path =
                    base + rank_folders[rank] + "/" + suit_folders[suit] + "/" +
                    rank_names[rank] + "_" + suit_names[suit] + ".png";

                card_animations[rank][suit] = esat::SpriteFromFile(path.c_str());
                assert(card_animations[rank][suit] != nullptr);

                for (int frame = 0; frame < 8; ++frame)
                {
                    card_animation_frames[rank][suit][frame] = esat::SubSprite(
                        card_animations[rank][suit],
                        frame * 135, 0,
                        135, 189);

                    assert(card_animation_frames[rank][suit][frame] != nullptr);
                }
            }
        }
    }
    void EsatView::drawFloatingCard(esat::SpriteHandle sprite, float x, float y, float phase) const
    {
        const double seconds = esat::Time() / 1000.0;
        const float offset_x = static_cast<float>(std::sin(seconds * 1.5 + phase)) * 3.0f;
        const float offset_y = static_cast<float>(std::cos(seconds * 1.2 + phase)) * 2.0f;
        esat::DrawSprite(sprite, x + offset_x, y + offset_y);
    }

    void EsatView::ReleaseSprites()
    {
        for (int i = 0; i < kRankCount; i++)
        {
            for (int j = 0; j < kSuitCount; j++)
            {
                if (card_sprites[i][j] != nullptr)
                {
                    esat::SpriteRelease(card_sprites[i][j]); // primero sprite
                    card_sprites[i][j] = nullptr;            // luego puntero si no pierdo la referencia
                }
            }
        }
        for (int i = 0; i < n_chips_types; i++)
        {
            esat::SpriteRelease(chips_sprites[i]);
            chips_sprites[i] = nullptr;
        }
        esat::SpriteRelease(backside_card_sprite);
        esat::SpriteRelease(background_card_sprite);
        esat::SpriteRelease(chip_stack_sprite);
    }

    const char *EsatView::value_to_char(blackjack::Rank valor) const
    {
        switch (valor)
        {
        case blackjack::Rank::Ace:
            return "as";
            break;
        case blackjack::Rank::Two:
            return "2";
            break;
        case blackjack::Rank::Three:
            return "3";
            break;
        case blackjack::Rank::Four:
            return "4";
            break;
        case blackjack::Rank::Five:
            return "5";
            break;
        case blackjack::Rank::Six:
            return "6";
            break;
        case blackjack::Rank::Seven:
            return "7";
            break;
        case blackjack::Rank::Eight:
            return "8";
            break;
        case blackjack::Rank::Nine:
            return "9";
            break;
        case blackjack::Rank::Ten:
            return "10";
            break;
        case blackjack::Rank::King:
            return "rey";
            break;
        case blackjack::Rank::Queen:
            return "reina";
            break;
        case blackjack::Rank::Jack:
            return "jota";
            break;
        default:
            assert("invalid card_type to convert to char");
            return "none";
            break;
        }
    }

    const char *EsatView::suit_to_char(blackjack::Suit suit) const
    {
        switch (suit)
        {
        case blackjack::Suit::Hearts:
            return "corazones";
            break;
        case blackjack::Suit::Diamonds:
            return "diamantes";
            break;
        case blackjack::Suit::Clubs:
            return "treboles";
            break;
        case blackjack::Suit::Spades:
            return "picas";
            break;
        default:
            assert("invalid card_palo to convert to char");
            return "none";
            break;
        }
    }
    esat::SpriteHandle EsatView::getSpriteFromCard(Card card)
    {
        int animation_index = -1;

        switch (card.rank)
        {
        case Rank::Ace:
            animation_index = 0;
            break;
        case Rank::King:
            animation_index = 1;
            break;
        case Rank::Queen:
            animation_index = 2;
            break;
        case Rank::Jack:
            animation_index = 3;
            break;
        default:
            break;
        }

        const int suit = static_cast<int>(card.suit);

        if (animation_index >= 0)
        {
            // Time() devuelve milsecs
            const int frame = static_cast<int>(esat::Time() / 125.0) % 8;
            return card_animation_frames[animation_index][suit][frame];
        }
        return card_sprites[static_cast<int>(card.rank) - 1][suit];
    }

    int EsatView::chooseRules(const IRules *const *rules, int count)
    {
        return -1;
    }
    IPlayer *EsatView::human()
    {
        return nullptr;
    }
    void EsatView::update(const ITable &table)
    {
    }
    void EsatView::draw(const ITable &table)
    {
        for (int seat = 0; seat < table.seatCount(); seat++)
        {
            for (int n_hand = 0; n_hand < table.handCount(seat); n_hand++)
            {
                HandState hand = table.hand(seat, n_hand);
                float x = spot_pos[seat].x;
                float y = spot_pos[seat].y + n_hand * 80;

                for (const Card &card : hand.cards)
                {
                    // elegir sprite segun rank y suit
                    esat::SpriteHandle sprite = getSpriteFromCard(card);
                    // dibujarlo en la posicion de ese puesto y mano
                    drawFloatingCard(sprite, x, y, x * 0.03f + y * 0.02f);
                    // desplazar la siguiente carta para que se vean ambas
                    x += 30.0f;
                }
            }
        }
        float x = esat::WindowWidth() / 2.0f - 64.0f;
        float y = esat::WindowHeight() / 2.0f - 120.0f;
        // cards visibles
        for (const Card &card : table.dealerUpCards())
        {
            drawFloatingCard(getSpriteFromCard(card), x, y, x * 0.03f + y * 0.02f);
            x += 30.0f;
        }

        // cards ocultas
        for (int i = 0; i < table.dealerHiddenCount(); ++i)
        {
            drawFloatingCard(backside_card_sprite, x, y, x * 0.03f + y * 0.02f);
            x += 30.0f;
        }
        const int seat = table.currentSeat();
        if (seat >= 0 && seat < table.seatCount())
        {
            printAlphaLerpRect({spot_pos[seat].x - 45, spot_pos[seat].y - 70}, 250, 300);
        }
    }
    bool EsatView::busy() const
    {
        return false;
    }
    bool EsatView::quit() const
    {
        return false;
    }
    void EsatView::onEvent(const GameEvent &event, const ITable &table)
    {
    }
}
