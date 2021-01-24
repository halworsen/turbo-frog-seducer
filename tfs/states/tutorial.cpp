#include "state.h"
#include "gamestates.h"
#include "resources/sprites.h"

extern Statemachine stm;
extern MenuState menu_state;

void TutorialState::Entered()
{
    return;
}

void TutorialState::Exited()
{
    return;
}

void TutorialState::Tick(uint32_t delta)
{
    return;
}

void TutorialState::ButtonEvent(uint8_t button, bool pressed)
{
    if (pressed)
    {
        stm.ChangeState(&menu_state);
    }
}

void TutorialState::Draw(Pokitto::Display display)
{
    // Draw the tutorial
    uint8_t tutorial_indices[42] = { 0 };
    for (int idx = 0; idx < 42; idx++)
    {
        tutorial_indices[idx] = SPRITE_TUTORIAL_TILES + idx;
    }
    Sprite tutorial = Sprite(7, 6, tutorial_indices);
    tutorial.DrawSprite(display, 0, 0);
}
