#ifndef DCBLOCKER_H_INCLUDED
#define DCBLOCKER_H_INCLUDED

#include "JuceHeader.h"

class DCBlocker
{
public:
    DCBlocker (float freq = 35.0f) :
        freq (freq)
    {}

    void reset (float sampleRate)
    {
        auto wn = MathConstants<float>::pi * freq / sampleRate;
        auto b0 = 1.0f / (1.0f + wn);
        R = (1.0f - wn) * b0;

        x1 = 0.0f;
        y1 = 0.0f;
    }

    inline float process (float x)
    {
        auto y = x - x1 + R * y1;

        x1 = x;
        y1 = y;

        return y;
    }

private:
    const float freq;
    float R = 0.995f;

    float x1 = 0.0f;
    float y1 = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCBlocker)
};

#endif // DCBLOCKER_H_INCLUDED
