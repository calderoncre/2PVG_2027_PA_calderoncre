
#include <string>
#include "esatView.hpp"
#include <cmath>
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/time.h"
#include "cassert"
#include "../game/hand_value.hpp"

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
    void EsatView::drawFireworks(int seat)
    {   
        const double transcurrido = esat::Time() - fireworks_start[seat];
        const double delays[3]= {0.0, 280.0, 550.0};
        const esat::Vec2 offsets[3] = {{-120.0f,-210.0f}, {20.0f, -125.0f}, {110.0f,0-160.0f}};
        for (int i = 0; i < 3; i++)
        {
            double tiempo_animacion = transcurrido - delays[i];
            if(tiempo_animacion < 0.0 || tiempo_animacion > 900.0)
                continue;
            int frame = static_cast<int>(tiempo_animacion / 100.0);
            float x = spot_pos[seat].x + offsets[i].x;
            float y = spot_pos[seat].y + offsets[i].y;
            esat::DrawSprite(sprites_fireworks[i][frame],x,y);
        }
        if (transcurrido >= 1450.0)
            fireworks_active[seat] = false;
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
        if (isMouseInsideRect(button_pos[0], 140, 58) && click)
        {
            current_move.action = Action::Split;
            printf("boton split!\n");
            return current_move;
        }
        // TODO buttons double
        if (isMouseInsideRect(button_pos[1], 140, 58) && click)
        {
            current_move.action = Action::Double;
            printf("boton double!\n");
            return current_move;
        }
        // TODO buttons hit
        if (isMouseInsideRect(button_pos[2], 140, 58) && click)
        {
            current_move.action = Action::Hit;
            printf("boton hit!\n");
            return current_move;
        }
        // TODO buttons stand
        if (isMouseInsideRect(button_pos[3], 140, 58) && click)
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
        puede_repartir = false;
        if (isMouseInsideRect(button_pos[0], 140, 58) && click)
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
        drawFloatingCard(button_sprites[4], button_pos[0].x, button_pos[0].y, button_pos[0].x * 0.2f + button_pos[0].y * 0.3f);

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
    
    void EsatView::printHandSumatory(const ITable &table) const
    {
        const esat::Vec2 positions[] = {
            {win_width / 2 - 300, win_height - 320}, // O || O O
            {100, win_height - 360},
            {100, 200},                          // | O O
            {win_width / 2 - 100, 60},           // O | O
            {win_width - 300, 200},              // O O |
            {win_width - 300, win_height - 360}, // || O O O
            {win_width / 2 + 100, win_height - 320}};

        for (int seat = 0; seat < table.seatCount(); seat++)
        {
            for (int n_hand = 0; n_hand < table.handCount(seat); n_hand++)
            {
                float x = positions[seat].x + 100;
                float y = positions[seat].y - 50;
                HandState hand = table.hand(seat, n_hand);
                std::string cadena = std::to_string(handValue(hand.cards));

                if (handValue(hand.cards) < 21)
                    printColoredSquare({x, y}, 0, 147, 57);
                else if (handValue(hand.cards) == 21)
                    printColoredSquare({x, y}, 233, 189, 21);
                else
                    printColoredSquare({x, y}, 140, 0, 0);

                esat::DrawSetStrokeColor(255, 255, 255, 255);
                esat::DrawSetFillColor(255, 255, 255, 255);
                esat::DrawText(x + 9, y + 26, cadena.c_str());
            }
        }
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
        int width = 50;
        int height = 35;
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
        button_sprites[4] = esat::SpriteFromFile("src/game/sprites/button_deal.png");

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
                    card_animation_frames[rank][suit][frame] = esat::SubSprite(card_animations[rank][suit], frame * 135, 0, 135, 189);
                    assert(card_animation_frames[rank][suit][frame] != nullptr);
                }
            }
        }

        //! FIREWORKS, EXPLOSION
        //! FIREWORKS
        const int n_firework_type = 3;
        const int n_firework_frame = 9;
        const char *firework[] = {"amarillo", "azul", "rojo"};
        const std::string fir_base = "src/game/sprites/";

        for (int i = 0; i < n_firework_type; ++i)
        {
            const std::string path = fir_base + "fireworks/" + firework[i] + ".png";
            sprite_sheet[i] = esat::SpriteFromFile(path.c_str());
            assert(sprite_sheet[i] != nullptr && "Error cargando fireworks");

            const int width = esat::SpriteWidth(sprite_sheet[i]);
            const int height = esat::SpriteHeight(sprite_sheet[i]);
            for (int frame = 0; frame < n_firework_frame; ++frame)
            {
                const int x = frame * width / n_firework_frame;
                const int next_x = (frame + 1) * width / n_firework_frame;

                sprites_fireworks[i][frame] = esat::SubSprite(sprite_sheet[i], x, 0, next_x - x, height);
                assert(sprites_fireworks[i][frame] != nullptr && "Error recortando fireworks");
            }
        }
        //! EXPLOSION
        // Usamos la posición 3, después de los tres fireworks.
        sprite_sheet[3] = esat::SpriteFromFile(
            "src/game/sprites/explosion/explosion.png");
        assert(sprite_sheet[3] != nullptr && "Error cargando explosion");
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                const int frame = col + 3 * row;
                sprite_explosion[frame] = esat::SubSprite(sprite_sheet[3], col * 418, row * 418, 418, 418);
                assert(sprite_explosion[frame] != nullptr && "Error recortando explosion");
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

    void EsatView::drawAnimationOnce(const esat::SpriteHandle *frames, float x, float y, double start_time)
    {
        
        double transcurrido = esat::Time() - start_time;
        if (transcurrido < 0.0 || transcurrido >= 900.0)
            return;

        int frame = static_cast<int>(transcurrido / 100.0);
        esat::DrawSprite(frames[frame], x, y);
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
        printHandSumatory(table);
        for (int i = 0; i < table.seatCount(); ++i)
        {
            if (explosion_active[i])
                drawAnimationOnce(sprite_explosion, spot_pos[i].x, spot_pos[i].y, explosion_start[i]);
            if(fireworks_active[i])
                drawFireworks(i);
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
        switch (event.type)
        {
        case EventType::HandBust:
            for (int i = 0; i < table.seatCount(); i++)
            {
                const int seat = event.seat;
                if (!explosion_active[seat])
                {
                    explosion_start[seat] = esat::Time();
                    explosion_active[seat] = true;
                }
                break;
            }
            break;
        case EventType::RoundStart:
            for (int i = 0; i < 7; ++i)
            {
                explosion_active[i] = false;
                fireworks_active[i] = false;
            }
            
            break;
        case EventType::OnBlackjack:
        {
            const int seat = event.seat;
            if (seat >= 0 && seat < 7)
            {
                fireworks_start[seat] = esat::Time();
                fireworks_active[seat] = true;
            }
        }
            break;
        default:
            break;
        }
    }
}
