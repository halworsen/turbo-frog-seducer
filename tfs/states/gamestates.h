#include "state.h"
#include "Pokitto.h"
#include "display/sprite.h"
#include "input.h"

class MenuState : public Gamestate {
private:
    uint8_t menu_choice = 0xFF;
public:
    void Entered();
    void Exited();
    void Tick(uint32_t delta);
    void Draw(Pokitto::Display display);
    void ButtonEvent(uint8_t button, bool pressed);
};

class PlayingState : public Gamestate {
private:
    // The player's sprite
    Sprite *player;
    // The mate's sprite
    Sprite *mate;
    // Effect sprite. Used when a frog is seduced
    Sprite *heart_effect;

    // Clock time at which we started the game
    uint32_t start_time = 0;

    // Array holding the 3 moves that need to be input to charm the frog
    uint8_t current_moves[3] = { 0x0 };
    // ;)
    uint8_t konami_level = 0;
    uint8_t konami_moves[10] = {
        BUTTON_UP, BUTTON_UP,
        BUTTON_DOWN, BUTTON_DOWN,
        BUTTON_LEFT, BUTTON_RIGHT,
        BUTTON_LEFT, BUTTON_RIGHT,
        BUTTON_B, BUTTON_A
    };

    // Which frog variant are we charming
    uint8_t current_mate = 0;
    // Current charm level. Represents how many correct, in-order inputs have been made
    uint8_t charm_level = 0;
    // Current love level. Represents how many movesets have been completed.
    // You need to complete 3 to seduce a frog.
    uint8_t love_level = 0;
    // How many frogs have been seduced
    uint16_t frogs_seduced = 0;

    // Small delay between seducing a frog and the next one appearing.
    uint32_t new_mate_time = 0;

    // Generate a new moveset
    void GenerateMoveset();

    // Generate a new mate
    void GenerateMate();

public:
    void Entered();
    void Exited();
    void Tick(uint32_t delta);
    void Draw(Pokitto::Display display);
    void ButtonEvent(uint8_t button, bool pressed);

    uint16_t GetScore();
};

class GameoverState : public Gamestate {
public:
    void Entered();
    void Exited();
    void Tick(uint32_t delta);
    void Draw(Pokitto::Display display);
    void ButtonEvent(uint8_t button, bool pressed);
};

class TutorialState : public Gamestate {
public:
    void Entered();
    void Exited();
    void Tick(uint32_t delta);
    void Draw(Pokitto::Display display);
    void ButtonEvent(uint8_t button, bool pressed);
};

