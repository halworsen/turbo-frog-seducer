#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include "Pokitto.h"

// Utility class that can handle sprites which consist of multiple tiles
// Doesn't care about palette.
class Sprite
{
private:
    // Armed with the powerful knowledge that I use 16x16 tiles... fuck it, magic numbers :/
    uint8_t tile_width = 16;
    uint8_t tile_height = 16;

    // Sprite position
    int8_t x = 0;
    int8_t y = 0;

    // Width and height of the sprite as amount of tiles
    uint8_t sprite_width;
    uint8_t sprite_height;

    // Array of tile indices which make up the sprite
    uint8_t *tile_indices;

    // Animation stuff
    uint32_t anim_start = 0;
    int8_t anim_tx = 0;
    int8_t anim_ty = 0;
    uint32_t anim_duration = 0;

    // Animator functions for x and y
    double (*animator_x)(double, uint32_t);
    double (*animator_y)(double, uint32_t);
public:
    Sprite(
        uint8_t sprite_width,
        uint8_t sprite_height,
        uint8_t *tile_indices
    );
    ~Sprite();

    // Sets the new sprite origin
    void SetOrigin(int8_t x, int8_t y);

    // Draws the sprite. Top-left origin if position is given.
    void DrawSprite(Pokitto::Display display, int8_t x, int8_t y);
    void DrawSprite(Pokitto::Display display);

    // Sets up a new animation to be played
    void StartAnimation(
        int8_t tx,
        int8_t ty,
        uint32_t duration,
        double (*animator_x)(double, uint32_t),
        double (*animator_y)(double, uint32_t)
    );
};

#endif