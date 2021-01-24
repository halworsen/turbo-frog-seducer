#ifndef INPUT_H
#define INPUT_H

// How many inputs there are in total
#define INPUT_COUNT 7

// Bitmasks for the different buttons
#define BUTTON_UP    (1 << 0)
#define BUTTON_RIGHT (1 << 1)
#define BUTTON_DOWN  (1 << 2)
#define BUTTON_LEFT  (1 << 3)
#define BUTTON_A     (1 << 4)
#define BUTTON_B     (1 << 5)
#define BUTTON_C     (1 << 6)

// Array with all inputs
const uint8_t pokitto_inputs[INPUT_COUNT] = { BUTTON_UP, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_LEFT, BUTTON_A, BUTTON_B, BUTTON_C };

#endif
