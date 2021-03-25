#include <stdint.h>
#include "display/sprite.h"
#include "Pokitto.h"

#include "resources/sprites.h"
#include "display/animators.h"

extern Pokitto::Core game;

extern const uint8_t sprites[243][258];

Sprite::Sprite(
        uint8_t sprite_width,
        uint8_t sprite_height,
        uint8_t *tile_indices
    )
{
    this->sprite_width = sprite_width;
    this->sprite_height = sprite_height;

    // Copy over tile indices so we don't lose them
    this->tile_indices = new uint8_t[sprite_width * sprite_height];
    std::copy(tile_indices, tile_indices + (sprite_width * sprite_height), this->tile_indices);
}

Sprite::~Sprite()
{
    delete[] this->tile_indices;
}

void Sprite::SetOrigin(int8_t x, int8_t y)
{
    this->x = x;
    this->y = y;
}

/**
 * Draw each tile of the sprite from top-left to bottom-right
 */
void Sprite::DrawSprite(Pokitto::Display display)
{
    int8_t anim_offset_x = 0;
    int8_t anim_offset_y = 0;
    // Check if the animation should still be going
    if (game.getTime() < (this->anim_start + this->anim_duration))
    {
        anim_offset_x = this->animator_x(game.getTime() - this->anim_start, this->anim_duration) * this->anim_tx;
        anim_offset_y = this->animator_y(game.getTime() - this->anim_start, this->anim_duration) * this->anim_ty;
    }

    for (int row = 0; row < this->sprite_height; row++)
    {
        for (int column = 0; column < this->sprite_width; column++)
        {
            int8_t draw_x = (column * this->tile_width + this->x) + anim_offset_x;
            // Origin is top-left, so movement along the Y-axis is "inverted"
            int8_t draw_y = (row * this->tile_height + this->y) - anim_offset_y;

            display.drawBitmap(
                draw_x,
                draw_y,
                sprites[this->tile_indices[column + row * this->sprite_width]]
            );
        }
    }
}

// lazy
void Sprite::DrawSprite(Pokitto::Display display, int8_t x, int8_t y)
{
    int8_t old_x = x;
    int8_t old_y = y;
    this->SetOrigin(x, y);
    this->DrawSprite(display);
    this->SetOrigin(old_x, old_y);
}

void Sprite::StartAnimation(
        int8_t tx,
        int8_t ty,
        uint32_t duration,
        double (*animator_x)(double, uint32_t),
        double (*animator_y)(double, uint32_t)
    )
{
    this->anim_start = game.getTime();
    this->anim_duration = duration;
    this->anim_tx = tx;
    this->anim_ty = ty;
    this->animator_x = animator_x;
    this->animator_y = animator_y;
}