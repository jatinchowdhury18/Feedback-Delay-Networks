#ifndef SMOOTHDELAY_H_INCLUDED
#define SMOOTHDELAY_H_INCLUDED

#include "DelayLine.h"

/** 
 * Delay line with fractional sample
 * delays and smooth delay length changes.
 * 
 * Based on Faust `fdelay`: https://github.com/grame-cncm/faustlibraries/blob/master/delays.lib#L89
 * */
class SmoothDelay
{
public:
    SmoothDelay();

    void setDelay (float lenMs);
    void reset (float sampleRate);

    inline void write (float data)
    {
        lineLow.write (data);
        lineHigh.write (data);
    }

    inline float read()
    {
        return delayFrac * lineHigh.read()
            + (1.0f - delayFrac) * lineLow.read();
    }

    inline void updatePtrs()
    {
        lineLow.updatePtrs();
        lineHigh.updatePtrs();

        if (smoothDelay.isSmoothing())
            updateDelay();
    }

    void updateDelay()
    {
        auto lenMs = smoothDelay.getNextValue();

        float delaySamp = (lenMs / 1000.0f) * fs;
        lineLow.setDelay ((int) delaySamp);
        lineHigh.setDelay ((int) delaySamp + 1);

        delayFrac = delaySamp - (int) delaySamp;
    }

private:
    DelayLine lineLow;
    DelayLine lineHigh;

    float fs = 44100.0f;
    float delayFrac = 0.0f;

    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothDelay = 10.0f;
    enum { smoothSteps = 500 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothDelay)
};

#endif // SMOOTHDELAY_H_INCLUDED
