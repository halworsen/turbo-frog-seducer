#include "states/gamestates.h"
#include "display/sprite.h"
#include "input.h"
#include "game.h"

#include <string>
#include <math.h>

#include "display/animators.h"

#define min(a, b) a < b ? a : b
// https://stackoverflow.com/questions/17944/how-to-round-up-the-result-of-integer-division
#define ceil_div(a, b) (a + (b - 1)) / b;

extern Statemachine stm;
extern GameoverState gameover_state;
extern Pokitto::Core game;

extern const uint8_t sprites[243][258];

void PlayingState::Entered()
{
    // pregame ends in 3 seconds
    this->start_time = game.getTime() + 3000;

    // Clean the slate in case this is a subsequent playthrough
    this->frogs_seduced = 0;
    this->charm_level = 0;
    this->love_level = 0;

    uint8_t player_indices[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    this->player = new Sprite(3, 3, player_indices);
    this->player->SetOrigin(2, 40);

    this->GenerateMate();
    this->GenerateMoveset();
}

void PlayingState::Exited()
{
    delete this->player;
    delete this->mate;
    // Heart effect may be up when the timer runs out
    if (this->heart_effect != nullptr)
        delete this->heart_effect;

    this->player = nullptr;
    this->mate = nullptr;
    this->heart_effect = nullptr;
}

void PlayingState::Tick(uint32_t delta)
{
    uint32_t cur_time = game.getTime();
    if (cur_time >= this->start_time + TIME_LIMIT)
    {
        stm.ChangeState(&gameover_state);
    }

    // Pick a new mate and reset charm/love levels. Make a new moveset too
    if(this->new_mate_time > 0 && cur_time > this->new_mate_time)
    {
        this->new_mate_time = 0;
        if (this->heart_effect != nullptr)
        {
            delete this->heart_effect;
            this->heart_effect = nullptr;
        }

        this->charm_level = 0;
        this->love_level = 0;
        this->GenerateMate();
        this->GenerateMoveset();
    }
}

void PlayingState::ButtonEvent(uint8_t button, bool pressed)
{
    // not really interested in release events (or the button C)
    if (!pressed || button == BUTTON_C)
    {
        return;
    }

    uint32_t cur_time = game.getTime();
    // Don't register inputs before the game actually starts
    if (cur_time < this->start_time)
    {
        return;
    }

    // Move animations!
    switch (button)
    {
    case BUTTON_RIGHT:
        this->player->StartAnimation(
            10, 0, 50,
            Animators::sine_half,
            Animators::sine_half
        );
        break;

    case BUTTON_LEFT:
        this->player->StartAnimation(
            -10, 0, 50,
            Animators::sine_half,
            Animators::sine_half
        );
        break;

    case BUTTON_UP:
        this->player->StartAnimation(
            0, 10, 50,
            Animators::sine_half,
            Animators::sine_half
        );
        break;

    case BUTTON_DOWN:
        this->player->StartAnimation(
            0, -10, 50,
            Animators::sine_half,
            Animators::sine_half
        );
        break;

    case BUTTON_A:
        this->player->StartAnimation(
            5, 10, 100,
            Animators::sine_half,
            Animators::sine_full
        );
        break;

    case BUTTON_B:
        this->player->StartAnimation(
            -5, -10, 100,
            Animators::sine_full,
            Animators::sine_half
        );
        break;
    
    default:
        break;
    }

    // We need to push all 3 buttons in the correct order
    // So what we can do is just index the moves that need to be input and compare it to the input
    // If they match, we advance the index. If not, the player messed up and we reset!
    if (button == this->current_moves[this->charm_level])
    {
        this->charm_level++;
    }
    else
    {
        this->charm_level = 0;
    }
    

    // We managed to charm a frog!
    if (this->charm_level == 3)
    {
        this->love_level++;

        // We *seduced* the frog
        if (this->love_level == 3)
        {
            this->frogs_seduced = min(this->frogs_seduced + 1, 999);
            // Choose a new mate in 0.6 seconds
            this->new_mate_time = cur_time + 600;
            // Little charm animation on the mate
            this->mate->StartAnimation(
                10, 0, 500,
                Animators::sine_full,
                Animators::sine_half
            );

            uint8_t heart_index[1] = { SPRITE_HEART };
            this->heart_effect = new Sprite(1, 1, heart_index);
            this->heart_effect->SetOrigin(60, 20);
            this->heart_effect->StartAnimation(
                10, 60, 600,
                Animators::sine_full,
                Animators::linear
            );
        }
        else
        {
            // Reset charm level and make a new moveset
            this->charm_level = 0;
            this->GenerateMoveset();
        }
    }

    // ;  )
    if (button == this->konami_moves[this->konami_level])
    {
        this->konami_level++;
        if (this->konami_level == 10)
        {
            this->frogs_seduced = 999;
        }
    }
    else
    {
        this->konami_level = 0;
    }
}

void PlayingState::Draw(Pokitto::Display display)
{
    // If we're in the pregame countdown, show a countdown
    uint32_t cur_time = game.getTime();
    if (cur_time < this->start_time)
    {
        display.setColor(PALETTE_COLOR_BLACK);
        display.fillRectangle(0, 0, display.width, display.height);
        int rounded_time_left = ceil_div(this->start_time - cur_time, 1000);
        std::string time_left = std::to_string(rounded_time_left) + "!";
        display.setColor(PALETTE_COLOR_WHITE);
        display.print(
            display.width / 2 - 3, display.height / 2 - 3,
            time_left.c_str()
        );

        return;
    }

    // Draw the background
    uint8_t bg_indices[42] = { 0 };
    std::copy(playing_background, playing_background + 42, bg_indices);
    Sprite background = Sprite(7, 6, bg_indices);
    background.DrawSprite(display, 0, 0);

    // Draw the player
    this->player->DrawSprite(display);
    // Draw the mate
    this->mate->DrawSprite(display);

    if (this->new_mate_time > 0 && this->heart_effect != nullptr)
        this->heart_effect->DrawSprite(display);

    // Draw the current moves
    for (int idx = 0; idx < 3; idx++)
    {
        uint8_t move = this->current_moves[idx];
        uint8_t done = this->charm_level > idx ? 1 : 0;
        display.drawBitmap(display.width - 18 * (3 - idx), display.height - 18, sprites[input_sprites[move][done]]);
    }

    // Draw a love bar
    uint8_t love_indices[3] = { SPRITE_HEART, 0 };
    switch (this->love_level)
    {
    case 0:
        love_indices[1] = SPRITE_LOVE_EMPTY1;
        love_indices[2] = SPRITE_LOVE_EMPTY2;
        break;
    case 1:
        love_indices[1] = SPRITE_LOVE_ONE1;
        love_indices[2] = SPRITE_LOVE_ONE2;
        break;
    case 2:
        love_indices[1] = SPRITE_LOVE_TWO1;
        love_indices[2] = SPRITE_LOVE_TWO2;
        break;
    case 3:
        love_indices[1] = SPRITE_LOVE_FULL1;
        love_indices[2] = SPRITE_LOVE_FULL2;
        break;

    default:
        break;
    }

    Sprite love = Sprite(3, 1, love_indices);
    love.DrawSprite(display, 0, 2);

    display.setColor(PALETTE_COLOR_BLACK);
    // Draw the current score
    std::string x = "x";
    std::string score = x + std::to_string(this->frogs_seduced);
    display.print(2, 12, score.c_str());

    // Draw time left
    uint32_t left = (this->start_time + TIME_LIMIT) - cur_time;
    uint32_t seconds = ceil_div(left, 1000);

    std::string minutes_str = "0" + std::to_string(seconds / 60);
    std::string seconds_str = std::to_string(seconds % 60);
    if ((seconds % 60) < 10)
        seconds_str = "0" + seconds_str;

    std::string time_remaining = minutes_str + ":" + seconds_str;
    display.print(2, 20, time_remaining.c_str());
}

void PlayingState::GenerateMoveset()
{
    // -1 omits C as a possible input
    this->current_moves[0] = pokitto_inputs[rand() % (INPUT_COUNT - 1)];
    this->current_moves[1] = pokitto_inputs[rand() % (INPUT_COUNT - 1)];
    this->current_moves[2] = pokitto_inputs[rand() % (INPUT_COUNT - 1)];
}

void PlayingState::GenerateMate()
{
    // Clean up the old mate if necessary
    if (this->mate != nullptr)
        delete this->mate;

    // Make sure we get a new mate. Not to promote variety, just to prevent confusion
    // Konami code unlocks Kermit and Crazy Frog
    uint8_t mate_modulo = (this->frogs_seduced == 999) ? 7 : 5;
    this->current_mate = (this->current_mate + 1 + (rand() % 4)) % mate_modulo;

    uint8_t mate_indices[9] = { 0 };
    for (int idx = 0; idx < 9; idx++)
    {
        mate_indices[idx] = 9 + (this->current_mate * 9) + idx;
    }

    this->mate = new Sprite(3, 3, mate_indices);
    this->mate->SetOrigin(60, 2);
}

uint16_t PlayingState::GetScore()
{
    return this->frogs_seduced;
}