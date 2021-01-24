#include "state.h"
#include "gamestates.h"
#include "input.h"
#include "resources/sprites.h"

extern Statemachine stm;
extern PlayingState playing_state;
extern TutorialState tutorial_state;

void MenuState::Entered()
{
    return;
}

void MenuState::Exited()
{
    return;
}

void MenuState::Tick(uint32_t delta)
{
    return;
}

void MenuState::ButtonEvent(uint8_t button, bool pressed)
{
    if (!pressed)
    {
        return;
    }

    // The game logic may read the A input after sound setup as a press
    // If we had started with menu_choice = 0, that'd just put the player
    // straight into the playing state. Not pleasant.
    if (this->menu_choice == 0xFF)
    {
        if (button == BUTTON_UP)
            this->menu_choice = 0;
        if (button == BUTTON_DOWN)
            this->menu_choice = 1;
    }

    if (button == BUTTON_A)
    {
        if (this->menu_choice == 0)
        {
            stm.ChangeState(&playing_state);
        }
        else if (this->menu_choice == 1)
        {
            stm.ChangeState(&tutorial_state);
        }
    }

    if (button == BUTTON_UP)
    {
        if (this->menu_choice > 0)
        {
            this->menu_choice--;
        }
    }

    if (button == BUTTON_DOWN)
    {
        if (this->menu_choice < 1)
        {
            this->menu_choice++;
        }
    }
}

void MenuState::Draw(Pokitto::Display display)
{
    // Draw the background
    uint8_t background_indices[42] = { 0 };
    for (int idx = 0; idx < 42; idx++)
    {
        background_indices[idx] = SPRITE_BACKGROUND_TILES + idx;
    }
    Sprite background = Sprite(7, 6, background_indices);
    background.DrawSprite(display, 0, 0);

    // Draw the game title
    uint8_t logo_indices[10] = { 0 };
    // The last tile bleeds over into tutorial sprites
    for (int idx = 0; idx < 10; idx++)
    {
        logo_indices[idx] = SPRITE_TITLE_LOGO + idx;
    }
    Sprite title = Sprite(5, 2, logo_indices);
    title.DrawSprite(display, 22, 5);

    // Menu options
    uint8_t play_indices[2] = { SPRITE_PLAY1, SPRITE_PLAY2 };
    uint8_t tutorial_indices[4] = { SPRITE_TUTORIAL1, SPRITE_TUTORIAL2, SPRITE_TUTORIAL3, SPRITE_TUTORIAL4 };
    if (this->menu_choice == 0)
    {
        play_indices[0] = SPRITE_PLAY1_SELECTED;
        play_indices[1] = SPRITE_PLAY2_SELECTED;
    }
    else if (this->menu_choice == 1)
    {
        tutorial_indices[0] = SPRITE_TUTORIAL1_SELECTED;
        tutorial_indices[1] = SPRITE_TUTORIAL2_SELECTED;
        tutorial_indices[2] = SPRITE_TUTORIAL3_SELECTED;
        tutorial_indices[3] = SPRITE_TUTORIAL4_SELECTED;
    }

    Sprite play_sprite = Sprite(2, 1, play_indices);
    Sprite tutorial_sprite = Sprite(4, 1, tutorial_indices);

    play_sprite.DrawSprite(display, 43, 40);
    tutorial_sprite.DrawSprite(display, 24, 60);
}
