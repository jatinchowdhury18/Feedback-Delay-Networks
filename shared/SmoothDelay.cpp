#include "SmoothDelay.h"

SmoothDelay::SmoothDelay()
{
}

void SmoothDelay::setDelay (float lenMs)
{
    smoothDelay = lenMs;
    updateDelay();
}

void SmoothDelay::reset (float sampleRate)
{
    fs = sampleRate;
    line.reset();

    for (int n = 0; n < 4; ++n)
        z[n] = 0.0f;

    readVal = 0.0f;
}
