#ifndef DELAYLINE_H_INCLUDED
#define DELAYLINE_H_INCLUDED

#include "JuceHeader.h"

/** Simple delay line with integer sample delay */
class DelayLine
{
public:
    DelayLine()
    {
        buffer.reset(new float [maxDelay]);
    }

    void setDelay (int lengthSamples) { delayLenSamples = (int) floor (jmin (lengthSamples, (int) maxDelay)); }

    void reset()
    {
        for (int n = 0; n < maxDelay; ++n)
            buffer[n] = 0.0f;
    }

    inline void write (float data) { buffer[wp] = data; }
    inline float read() { return buffer[rp]; }

    inline void updatePtrs()
    {
        wp -= 1;

        if (wp < 0) // wrap write pointer
            wp = maxDelay - 1;

        rp = wp + delayLenSamples;
        if (rp >= maxDelay) // wrap read pointer
            rp -= maxDelay;
    }

private:
    enum
    {
        maxDelay = 64*8192,
    };

    std::unique_ptr<float[]> buffer;
    int wp = 0; // write pointer
    int rp = 0; // read pointer
    int delayLenSamples = 0; // delay length in samples

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLine)
};

#endif //DELAYLINE_H_INCLUDED
