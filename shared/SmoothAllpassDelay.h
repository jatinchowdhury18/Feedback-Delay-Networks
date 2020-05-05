#ifndef SMOOTHALLPASSDELAY_H_INCLUDED
#define SMOOTHALLPASSDELAY_H_INCLUDED

#include "SmoothDelay.h"

class SmoothAllpassDelay : public SmoothDelay
{
public:
    SmoothAllpassDelay() {}

    void reset (float sampleRate) override
    {
        SmoothDelay::reset (sampleRate);

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
        curY = -g * curX + SmoothDelay::read();

        auto in = g * curY + curX;
        SmoothDelay::write (in);

        SmoothDelay::updatePtrs();
    }

private:
    float curX = 0.0f;
    float curY = 0.0f;

    float g = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothAllpassDelay)
};

#endif // SMOOTHALLPASSDELAY_H_INCLUDED
