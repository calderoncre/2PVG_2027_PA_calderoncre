#include <string>
#include <cassert>
#include "esat/window.h"
#include "esat/draw.h"
#include "esat/sprite.h"
#include "esat/input.h"
#include "esat/time.h"
#include "IView.hpp"

void UI::LoadSprites()
{
    std::string cadena = "";
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            // le estoy diciendo al compilador: interpreta este entero como uno de los valores de mi enum
            Type type = static_cast<Type>(i);
            Color color = static_cast<Color>(j);
            //! dynamic cast
            if (type == Type::Ctmas4 || type == Type::Cambiar_color) // salvar casos de +4 y cambiar color
            {
                if (color != Color::Null)
                    continue;
            }
            else
            {
                if (color == Color::Null)
                    continue;
            }
            cadena.append("sprites/");
            // cadena.append(type_to_char(getType(i))); //! no podia hacer esto claro
            cadena.append(type_to_char(type));
            cadena.append("_");
            cadena.append(color_to_char(color));
            cadena.append(".png");
            printf("%s \n", cadena.c_str());
            card_sprites[i][j] = esat::SpriteFromFile(cadena.c_str());
            cadena.clear();
        }
    }
    backside_card_sprite = esat::SpriteFromFile("sprites/backside_card2.png");
}
void UI::ReleaseSprites()
{
    int n_types = 15;
    int n_colors = 5;
    for (int i = 0; i < n_types; i++)
    {
        for (int j = 0; j < n_colors; j++)
        {
            if (card_sprites[i][j] != nullptr)
            {
                esat::SpriteRelease(card_sprites[i][j]); // primero sprite
                card_sprites[i][j] = nullptr;            // luego puntero si no pierdo la referencia
            }
        }
    }
}

esat::SpriteHandle UI::getCardSprite(const Type type, const Color color) const
{
    return card_sprites[static_cast<int>(type)][static_cast<int>(color)];
}

// void Graphic_interface::printClickableRectangles(Deck deck)
// {

// }
// bool isMouseInsideCard()
// {

// }

char *UI::type_to_char(Type type) const
{
    switch (type)
    {
    case Type::Ct0:
        return "0";
        break;
    case Type::Ct1:
        return "1";
        break;
    case Type::Ct2:
        return "2";
        break;
    case Type::Ct3:
        return "3";
        break;
    case Type::Ct4:
        return "4";
        break;
    case Type::Ct5:
        return "5";
        break;
    case Type::Ct6:
        return "6";
        break;
    case Type::Ct7:
        return "7";
        break;
    case Type::Ct8:
        return "8";
        break;
    case Type::Ct9:
        return "9";
        break;
    case Type::Ctmas2:
        return "ctmas2";
        break;
    case Type::Ctmas4:
        return "ctmas4";
        break;
    case Type::Prohibido:
        return "prohibido";
        break;
    case Type::Cambiar_direccion:
        return "cambiar_direccion";
        break;
    case Type::Cambiar_color:
        return "cambiar_color";
        break;
    default:
        return "none";
        assert("invalid card_type to convert to char");
        break;
    }
}

char *UI::color_to_char(Color color) const
{
    switch (color)
    {
    case Color::Red:
        return "red";
        break;
    case Color::Green:
        return "green";
        break;
    case Color::Yellow:
        return "yellow";
        break;
    case Color::Blue:
        return "blue";
        break;
    default:
        return "none";
        assert("invalid card_color to convert to char");
        break;
    }
}

void UI::printCard(float x, float y, const ICard &card) const
{
    esat::SpriteHandle sprite = getCardSprite(card.getType(), card.getColor());
    assert(sprite != nullptr && "trying to draw null sprite");
    esat::DrawSprite(sprite, x, y);
}

void UI::printPlayerDeck(const IDeck &deck, esat::Vec2 pos) const
{
    esat::Vec2 pos_final;
    for (int i = 0; i < deck.size(); i++)
    {
        pos_final = cardPosition(i, pos);

        printCard(pos_final.x, pos_final.y, deck[i]);
        printSelectableRectangles(pos_final, 66, 85);
    }
}
esat::Vec2 UI::cardPosition(int index, esat::Vec2 origin) const
{
    esat::Vec2 tmp = {0, 0};
    float x = 20 + (index % 10) * 90;
    float y = 20 + (index / 10) * 85;
    tmp.x = origin.x + x;
    tmp.y = origin.y + y;
    return tmp;
}

void UI::printSelectableRectangles(esat::Vec2 pos, float ancho, float alto) const
{
    if (!isMouseInsiderRectangle(pos, ancho, alto))
        return;

    float points[8] = {
        pos.x, pos.y,
        pos.x + ancho, pos.y,
        pos.x + ancho, pos.y + alto,
        pos.x, pos.y + alto};
    esat::DrawSetFillColor(255, 255, 255, 50);
    esat::DrawSetStrokeColor(255, 255, 255, 50);
    esat::DrawSolidPath(points, 4);
    esat::DrawSetFillColor(255, 255, 255, 255);
}

bool UI::isMouseInsiderRectangle(esat::Vec2 pos, float ancho, float alto) const
{
    float mouseX = esat::MousePositionX();
    float mouseY = esat::MousePositionY();
    return mouseX >= pos.x && mouseX <= pos.x + ancho &&
           mouseY >= pos.y && mouseY <= pos.y + alto;
}

const ICard *UI::cardSelected() const
{
    return selected_card_;
}

void UI::clearSelections()
{
    if (selected_card_ != nullptr)
        selected_card_ = nullptr;

    index_selec_card = -1;
}
void UI::setEsatText() const
{
    esat::DrawSetStrokeColor(0, 255, 0, 255);
    esat::DrawSetFillColor(255, 255, 255, 255);
    esat::DrawSetTextFont("mi_fuente2.ttf");
    esat::DrawSetTextSize(20.0f);
}
void UI::printRivalPlayer() const
{
    // imprimir texto jugador "player + numero"
    for (int i = 0; i < model_.playerCount(); i++)
    {
        std::string rival_name = "Player ";
        rival_name += std::to_string(i);
        esat::DrawText(10, 120 * i + (i * 3) + 190, rival_name.c_str());

        for (int j = 0; j < model_.getPlayerHand(i).size(); j++)
        {
            esat::DrawSprite(backside_card_sprite, (j * 11) + 10, 120 * i + (i * 3) + 200);
        }
    }

    // imprimir sprites carta backside n veces por cada carta en el mazo de los demas jugadores moviendolos en la posicion x
}
void UI::printTextButtons() const
{
    esat::DrawText(500, 900, "UNO");
    esat::DrawText(600, 900, "CHALLENGE");
    esat::DrawText(770, 900, "ACUSE");
}
void UI::printButtons() const
{
    // UNO,
    printSelectableRectangles({480, 880}, 80, 25);
    // Acusse
    printSelectableRectangles({580, 880}, 155, 25);
    // Challenge,
    printSelectableRectangles({750, 880}, 104, 25);

    // colores
    if (colorPalette_active)
    {
        printSelectableRectangles({750, 650}, 50, 50);
        printSelectableRectangles({800, 650}, 50, 50);
        printSelectableRectangles({750, 700}, 50, 50);
        printSelectableRectangles({800, 700}, 50, 50);
    }

    // Draw
    printSelectableRectangles({380, 330}, 104, 127);
}
bool UI::drawWanted() const
{
    if (isMouseInsiderRectangle({380, 330}, 104, 127) && esat::MouseButtonDown(0))
        return true;
    return false;
}
BoolAction UI::wantChallenge() const
{
    //! faltaran condiciones
    if (isMouseInsiderRectangle({750, 880}, 104, 25) && esat::MouseButtonDown(0))
        return BoolAction::Yes;
    return BoolAction::Null;
}
BoolAction UI::unoSaid() const
{
    //! faltaran condiciones
    if (isMouseInsiderRectangle({480, 880}, 80, 25) && esat::MouseButtonDown(0))
        return BoolAction::Yes;
    return BoolAction::Null;
}

Color UI::colorSelected() const
{
    if (!colorPalette_active)
        return Color::Null;

    if (isMouseInsiderRectangle({750, 650}, 50, 50) && esat::MouseButtonDown(0))
        return Color::Red;
    if (isMouseInsiderRectangle({800, 650}, 50, 50) && esat::MouseButtonDown(0))
        return Color::Green;
    if (isMouseInsiderRectangle({750, 700}, 50, 50) && esat::MouseButtonDown(0))
        return Color::Blue;
    if (isMouseInsiderRectangle({800, 700}, 50, 50) && esat::MouseButtonDown(0))
        return Color::Yellow;
    return Color::Null;
}

bool UI::accuseUno() const
{
    //! faltaran condiciones
    if (isMouseInsiderRectangle({580, 880}, 155, 25) && esat::MouseButtonDown(0))
        return true;
    return false;
}
void UI::printColoredSquare(esat::Vec2 pos, int r, int g, int b) const
{
    int ancho_alto = 50;
    float points[8]{
        pos.x, pos.y,
        pos.x + ancho_alto, pos.y,
        pos.x + ancho_alto, pos.y + ancho_alto,
        pos.x, pos.y + ancho_alto};
    esat::DrawSetFillColor(r, g, b, 255);
    esat::DrawSolidPath(points, 4);
}
void UI::activateColorPalette()
{
    if (!colorPalette_active)
        colorPalette_active = true;
}
void UI::desactivateColorPalette()
{
    if (colorPalette_active)
        colorPalette_active = false;
}
void UI::printCurrentColor(esat::Vec2 pos, Color color) const
{
    switch (color)
    {
    case Color::Red:
        printColoredSquare({pos.x, pos.y}, 255, 0, 0);
        break;
    case Color::Green:
        printColoredSquare({pos.x, pos.y}, 0, 255, 0);
        break;
    case Color::Blue:
        printColoredSquare({pos.x, pos.y}, 0, 0, 255);
        break;
    case Color::Yellow:
        printColoredSquare({pos.x, pos.y}, 255, 255, 0);
        break;
        /* code */

    default:
        assert("trying to print Null color");
        break;
    }
    esat::DrawSetFillColor(255, 255, 255, 255);
    esat::DrawSetStrokeColor(255, 255, 255, 255);
}

void UI::print() const
{

    const IDeck &discard = model_.getDiscardDeck();
    printPlayerDeck(model_.getPlayerHand(model_.currentPlayer()), {0, 0});
    if (discard.size() > 0)
    {
        printCard(esat::WindowWidth() / 2, esat::WindowHeight() / 2, discard[discard.size() - 1]);
    }

    printCurrentColor({600, 600}, model_.get_playable_color());

    esat::DrawSetFillColor(255, 255, 255, 255);
    esat::DrawSetStrokeColor(255, 255, 255, 255);
    printRivalPlayer();
    printButtons();
    printTextButtons();
    esat::DrawSprite(backside_card_sprite, esat::WindowWidth() / 2 - 100, esat::WindowHeight() / 2 - 150);

    if (colorPalette_active)
    {
        printColoredSquare({750, 650}, 255, 0, 0);
        printColoredSquare({800, 650}, 0, 255, 0);
        printColoredSquare({750, 700}, 0, 0, 255);
        printColoredSquare({800, 700}, 255, 255, 0);
    }

    esat::DrawSetFillColor(255, 255, 255, 255);
}

void UI::update()
{
    print();
    printMessages();
    if (colorPalette_active)
        return;
    // !card buttons current player
    for (int i = 0; i < model_.getPlayerHand(model_.currentPlayer()).size(); i++)
    {
        if (selected_card_ != nullptr)
            break;
        esat::Vec2 pos_final = cardPosition(i, {0, 0});
        if (!isMouseInsiderRectangle(pos_final, 66, 85))
            continue;
        if (esat::MouseButtonDown(0))
        {
            printf("%p\n", const_cast<void *>(static_cast<const void *>(selected_card_)));
            selected_card_ = &(model_.getPlayerHand(model_.currentPlayer()).operator[](i));
            index_selec_card = i;
            activateMessage("Card Selected!");
            printf("%p\n", const_cast<void *>(static_cast<const void *>(selected_card_)));
            break;
        }
    }

    //! UNO, challenge, accusee

    // if unoSaid
    // if accusse uno
    //  if wantChallenge
    //...

    //! square colors
}

int UI::get_index_selectedCard() const
{
    return index_selec_card;
}

void UI::activateMessage(std::string message)
{
    current_message_ = message;
    message_time_ = esat::Time();
    active_message_ = true;
}

void UI::printMessages()
{
    if (!active_message_)
        return;
    double cooldown = 3000.0f;
    if (esat::Time() - message_time_ < cooldown)
    {
        esat::DrawText(700, 800, current_message_.c_str());
    }
    else
        active_message_ = false;
}