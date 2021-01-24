#include <stdint.h>
#include "Pokitto.h"
#include "state.h"
#include "input.h"
#include "display/sprite.h"
#include "resources/sprites.h"

void Statemachine::ChangeState(Gamestate *new_state)
{
    this->current_state = new_state;
    new_state->Entered();
}

/**
 * Reads button states and calls button events in the current state if necessary
 * Huge, fat, ugly function full of boilerplate-y code
 * 
 * You could simplify this so much by writing a function that takes the button and its bitmask
 * BUT get this: that takes up more program memory
 */
void Statemachine::HandleInput(Pokitto::Buttons buttons)
{
    uint8_t new_mask = 0;

    if (buttons.upBtn())
    {
        // Check for a rising edge/pressed event (i.e. if the input mask had a 0 for button up last tick)
        if (~this->input_mask & BUTTON_UP)
        {
            this->current_state->ButtonEvent(BUTTON_UP, true);
        }

        new_mask |= BUTTON_UP;
    }
    else
    {
        // Check for a falling edge/released event (i.e. if the input mask had a 0 for button up last tick)
        if (this->input_mask & BUTTON_UP)
        {
            this->current_state->ButtonEvent(BUTTON_UP, false);
        }
    }
    

    if (buttons.rightBtn())
    {
        if (~this->input_mask & BUTTON_RIGHT)
        {
            this->current_state->ButtonEvent(BUTTON_RIGHT, true);
        }
        
        new_mask |= BUTTON_RIGHT;
    }
    else
    {
        if (this->input_mask & BUTTON_RIGHT)
        {
            this->current_state->ButtonEvent(BUTTON_RIGHT, false);
        }
    }

    if (buttons.downBtn())
    {
        if (~this->input_mask & BUTTON_DOWN)
        {
            this->current_state->ButtonEvent(BUTTON_DOWN, true);
        }
        
        new_mask |= BUTTON_DOWN;
    }
    else
    {
        if (this->input_mask & BUTTON_DOWN)
        {
            this->current_state->ButtonEvent(BUTTON_DOWN, false);
        }
    }

    if (buttons.leftBtn())
    {
        if (~this->input_mask & BUTTON_LEFT)
        {
            this->current_state->ButtonEvent(BUTTON_LEFT, true);
        }
        
        new_mask |= BUTTON_LEFT;
    }
    else
    {
        if (this->input_mask & BUTTON_LEFT)
        {
            this->current_state->ButtonEvent(BUTTON_LEFT, false);
        }
    }

    if (buttons.aBtn())
    {
        if (~this->input_mask & BUTTON_A)
        {
            this->current_state->ButtonEvent(BUTTON_A, true);
        }
        
        new_mask |= BUTTON_A;
    }
    else
    {
        if (this->input_mask & BUTTON_A)
        {
            this->current_state->ButtonEvent(BUTTON_A, false);
        }
    }

    if (buttons.bBtn())
    {
        if (~this->input_mask & BUTTON_B)
        {
            this->current_state->ButtonEvent(BUTTON_B, true);
        }
        
        new_mask |= BUTTON_B;
    }
    else
    {
        if (this->input_mask & BUTTON_B)
        {
            this->current_state->ButtonEvent(BUTTON_B, false);
        }
    }

    if (buttons.cBtn())
    {
        if (~this->input_mask & BUTTON_C)
        {
            this->current_state->ButtonEvent(BUTTON_C, true);
        }
        
        new_mask |= BUTTON_C;
    }
    else
    {
        if (this->input_mask & BUTTON_C)
        {
            this->current_state->ButtonEvent(BUTTON_C, false);
        }
    }

    this->input_mask = new_mask;
}

void Statemachine::Tick(uint32_t delta)
{
    if (this->current_state == nullptr)
    {
        return;
    }

    this->current_state->Tick(delta);
}

void Statemachine::Draw(Pokitto::Display display)
{
    if (this->current_state == nullptr)
    {
        return;
    }

    this->current_state->Draw(display);
}