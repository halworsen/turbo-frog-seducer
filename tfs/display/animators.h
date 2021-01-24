#ifndef ANIMATORS_H
#define ANIMATORS_H

#include <stdint.h>
#include <math.h>

class Animators
{
public:
    /**
     * Animates linearly from 0 to 1 over the given duration
     */
    static double linear(double t, uint32_t duration)
    {
        return (t / duration);
    }

    /**
     * Animates according to a sine wave. Ensures that a quarter period (0->1) is exactly duration milliseconds
     */
    static double sine_quarter(double t, uint32_t duration)
    {
        return sin((2*PI / (duration * 4)) * t);
    }

    /**
     * Animates according to a sine wave. Ensures that a half period (0->1->0) is exactly duration milliseconds
     */
    static double sine_half(double t, uint32_t duration)
    {
        return sin((2*PI / (duration * 2)) * t);
    }

    /**
     * Animates according to a sine wave. Ensures that a full period (0->1->0->-1->0) is exactly duration milliseconds
     */
    static double sine_full(double t, uint32_t duration)
    {
        return sin((2*PI / duration) * t);
    }
};

#endif