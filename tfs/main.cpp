#include <stdint.h>
#include "Pokitto.h"
#include "game.h"
#include "state.h"
#include "states/gamestates.h"

#include "display/sprite.h"
#include "resources/sprites.h"

Pokitto::Core game;

Statemachine stm;
MenuState menu_state;
PlayingState playing_state;
GameoverState gameover_state;
TutorialState tutorial_state;

int main() {
    // Setup the statemachine
    stm.ChangeState(&menu_state);

    game.begin();
    game.setFrameRate(GAME_FRAMERATE);

    game.display.load565Palette(sprites_pal);
    game.display.invisiblecolor = 0x0;
    game.display.bgcolor = 0x0;
    game.display.setColor(PALETTE_COLOR_BLACK);

    Pokitto::Display::setFont(fontTight);

    // Core game loop
    uint32_t last_frame_time = game.getTime();
    while (game.isRunning())
    {
        uint32_t cur_time = game.getTime();
        uint32_t delta = cur_time - last_frame_time;
        last_frame_time = cur_time;

        if (game.update()) {
            // Allow the state machine to handle inputs first
            stm.HandleInput(game.buttons);
            // Allow the state machine to tick (logic)
            stm.Tick(delta);
            // Draw to the screen
            stm.Draw(game.display);
        }
    }

    return 0;
}