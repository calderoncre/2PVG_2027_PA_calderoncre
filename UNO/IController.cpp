#include "IController.hpp"
void Controller::run()
{
    ui.update();
    // mientras espero el color solo proceso la paletaa
    if (model.get_awaiting_color())
    {
        Color chosen = ui.colorSelected();

        if (model.chooseColor(chosen))
        {
            ui.desactivateColorPalette();
            ui.clearSelections();
        }
        return;
    }

    int selected = ui.get_index_selectedCard();
    if (selected >= 0)
    {
        bool accepted = model.playCard(selected, false);
        ui.clearSelections();

        if (accepted)
        {
            ui.activateMessage("Carta correcta !!!");
            if (model.get_awaiting_color())
                ui.activateColorPalette();
        }
        else
            ui.activateMessage("Carta incorrecta !!!");
        return;
    }

    if (ui.drawWanted())
    {
        model.drawCard();
        ui.clearSelections();
    }
}
void Controller::init_game()
{
    //! UI
    ui.LoadSprites();
    ui.setEsatText();
    //! Modelo
    model.set_playerCount(4);
    model.reserve_memory_player_decks();
    model.set_turn();
    model.set_draw_deck();
    model.get_draw_deck().barajar_Reordenar();

    // inicializar mazos de jugadores. 7 cartas cada uno
    for (int i = 0; i < model.playerCount(); i++)
    {
        model.build_player_deck(model.get_draw_deck(), i);
    }

    // añadir primera carta a mazo descarte
    Card card(Type::Ct0, Color::Red);
    do
    {
        card = model.get_draw_deck().extractLastCard();
        model.get_discard_deck().addCard(card);
    } while (card.getColor() == Color::Null || card.getType() == Type::Ctmas2 ||
             card.getType() == Type::Prohibido || card.getType() == Type::Cambiar_direccion);

    // setear el color jugable
    model.set_playable_color(card);
}