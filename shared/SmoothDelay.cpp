#include "SmoothDelay.h"

SmoothDelay::SmoothDelay()
{
    smoothDelay.reset (smoothSteps);
}

void SmoothDelay::setDelay (float lenMs)
{
    smoothDelay.setTargetValue (lenMs);
}

void SmoothDelay::reset (float sampleRate)
{
    fs = sampleRate;
    lineLow.reset();
    lineHigh.reset();

    setDelay (smoothDelay.skip (smoothSteps));
}
