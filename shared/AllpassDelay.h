#ifndef ALLPASSDELAY_H_INCLUDED
#define ALLPASSDELAY_H_INCLUDED

#include "DelayLine.h"

/**
 * Allpass delay line.
 * Based on: https://ccrma.stanford.edu/~jos/pasp/Allpass_Two_Combs.html
 **/
class AllpassDelay : public DelayLine
{
public:
    AllpassDelay() {}

    void reset() override
    {
        DelayLine::reset();

        curX = 0.0f;
        curY = 0.0f;
    }

    void setG (float newG) { g = newG; }

    inline void write (float data) override
    {
        curX = data;
    }

    inline float read() const noexcept override
    {
        return curY;
    }

    inline void updatePtrs() override
    {
        curY = -g * curX + DelayLine::read();

        auto in = g * curY + curX;
        DelayLine::write (in);

        DelayLine::updatePtrs();
    }

private:
    float curX = 0.0f;
    float curY = 0.0f;

    float g = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllpassDelay)
};

#endif // ALLPASSDELAY_H_INCLUDED
