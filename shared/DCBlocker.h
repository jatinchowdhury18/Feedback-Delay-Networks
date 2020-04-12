#ifndef DCBLOCKER_H_INCLUDED
#define DCBLOCKER_H_INCLUDED

#include "JuceHeader.h"

/** 
 * First order DC blocking filter.
 * Useful for avoiding DC build-up in delay lines.
 * For more information see: https://ccrma.stanford.edu/~jos/filters/DC_Blocker.html
 * */
class DCBlocker
{
public:
    DCBlocker (float freq = 35.0f) :
        freq (freq)
    {}

    /** 
     * Recomputes filter coefficients for sample rate
     * and resets filter state.
     * */
    void reset (float sampleRate)
    {
        // coefficient calculation derived from
        // Faust DCBlocker: https://github.com/grame-cncm/faustlibraries/blob/master/filters.lib#L121
        auto wn = MathConstants<float>::pi * freq / sampleRate;
        auto b0 = 1.0f / (1.0f + wn);
        R = (1.0f - wn) * b0;

        x1 = 0.0f;
        y1 = 0.0f;
    }

    /** Process a single sample */
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
