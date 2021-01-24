#include "state.h"
#include "input.h"
#include "gamestates.h"
#include "resources/sprites.h"
#include <string>

extern Statemachine stm;
extern MenuState menu_state;
extern PlayingState playing_state;

void GameoverState::Entered()
{
    return;
}

void GameoverState::Exited()
{
    return;
}

void GameoverState::Tick(uint32_t delta)
{
    return;
}

void GameoverState::ButtonEvent(uint8_t button, bool pressed)
{
    if (button == BUTTON_C && pressed)
    {
        stm.ChangeState(&menu_state);
    }
}

void GameoverState::Draw(Pokitto::Display display)
{
    // Draw the background
    uint8_t background_indices[42] = { 0 };
    for (int idx = 0; idx < 42; idx++)
    {
        background_indices[idx] = SPRITE_BACKGROUND_TILES + idx;
    }
    Sprite background = Sprite(7, 6, background_indices);
    background.DrawSprite(display, 0, 0);

    // Fluff
    uint8_t sunset_indices[4] = { SPRITE_SUNSET1, SPRITE_SUNSET2, SPRITE_SUNSET3, SPRITE_SUNSET4 };
    Sprite sunset = Sprite(4, 1, sunset_indices);

    uint8_t seduced_indices[3] = { SPRITE_SEDUCED1, SPRITE_SEDUCED2, SPRITE_SEDUCED3 };
    Sprite seduced = Sprite(3, 1, seduced_indices);

    uint8_t continue_indices[3] = { SPRITE_CONTINUE1, SPRITE_CONTINUE2, SPRITE_CONTINUE3 };
    Sprite continue_text = Sprite(3, 1, continue_indices);

    sunset.DrawSprite(display, 3, 3);

    seduced.DrawSprite(display, 3, 16);
    display.drawLine(3, 24, 50, 24);
    string score = "x" + to_string(playing_state.GetScore());
    display.print(3, 27, score.c_str());

    display.drawBitmap(23, 72, sprites[SPRITE_BUTTON_C_YELLOW]);
    continue_text.DrawSprite(display, 43, 77);
}
