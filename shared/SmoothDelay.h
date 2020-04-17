#ifndef SMOOTHDELAY_H_INCLUDED
#define SMOOTHDELAY_H_INCLUDED

#include "DelayLine.h"

/** 
 * Delay line with fractional sample
 * delays using 4-point Lagrange interpolation.
 * 
 * Based on: https://ccrma.stanford.edu/~jos/pasp/Explicit_Lagrange_Coefficient_Formulas.html
 * */
class SmoothDelay
{
public:
    SmoothDelay();

    void setDelay (float lenMs);
    void reset (float sampleRate);

    inline void write (float data)
    {
        line.write (data);
    }

    inline float read()
    {
        return readVal;
    }

    inline void updatePtrs()
    {
        line.updatePtrs();

        readVal = h[0] * *line.readPtr + h[1] * z[1]+ h[2] * z[2] + h[3] * z[3];
        z[3] = z[2];
        z[2] = z[1];
        z[1] = *line.readPtr;
    }

    inline void updateDelay()
    {
        float delaySamp = (smoothDelay / 1000.0f) * fs;
        line.setDelay ((int) delaySamp);

        delayFrac = delaySamp - (int) delaySamp;
        h[0] = -1.0f * (delayFrac - 1.0f) * (delayFrac - 2.0f) * (delayFrac - 3.0f) / 6.0f;
        h[1] =  delayFrac * (delayFrac - 2.0f) * (delayFrac - 3.0f) / 2.0f;
        h[2] = -delayFrac * (delayFrac - 1.0f) * (delayFrac - 3.0f) / 2.0f;
        h[3] =  delayFrac * (delayFrac - 1.0f) * (delayFrac - 2.0f) / 6.0f;
    }

    float readVal;

private:
    DelayLine line;

    float fs = 44100.0f;
    float delayFrac = 0.0f;
    float h[4];
    float z[4];

    float smoothDelay = 10.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothDelay)
};

#endif // SMOOTHDELAY_H_INCLUDED
