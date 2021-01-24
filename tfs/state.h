#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include "Pokitto.h"

class Gamestate {
public:
    // Called when the state is entered
    virtual void Entered() = 0;
    // Called when the state is exited
    virtual void Exited() = 0;
    // Called when the state should tick (i.e. update game logic)
    virtual void Tick(uint32_t delta) = 0;
    // Called when the state should draw to the screen
    virtual void Draw(Pokitto::Display display) = 0;
    // Called when a button has been pressed or released
    virtual void ButtonEvent(uint8_t button, bool pressed) = 0;
};

// The big mother of all states
class Statemachine {
public:
    // Change the game state
    // This will call Entered() in the new state
    void ChangeState(Gamestate *new_state);

    // Called every game tick before Tick to gather this frame's inputs
    void HandleInput(Pokitto::Buttons buttons);

    // Called every game tick to compute logic
    void Tick(uint32_t delta);

    // Called after every game logic tick to draw the game
    void Draw(Pokitto::Display display);

private:
    Gamestate *current_state;
    // Bitmask storing button states. For example:
    // -- CBARDLU
    // 00 1010001 <- Dpad Up, A and C are held
    uint8_t input_mask;
};

#endif