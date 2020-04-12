#ifndef DELAYLINE_H_INCLUDED
#define DELAYLINE_H_INCLUDED

#include "JuceHeader.h"

/** Simple delay line with integer sample delay */
class DelayLine
{
public:
    DelayLine() {}

    void setDelay (int lengthSamples) { delayLenSamples = (int) floor (jmin (lengthSamples, (int) maxDelay)); }

    /** Reset delay buffer to 0 */
    void reset()
    {
        for (int n = 0; n < maxDelay; ++n)
            buffer[n] = 0.0f;

        readPtr = &buffer[rp];
        writePtr = &buffer[wp];
    }

    inline void write (float data)
    { 
        buffer[wp] = data;
    }
    
    inline float read() const noexcept
    { 
        return buffer[rp];
    }

    /** Update read and write pointers */
    inline void updatePtrs()
    {
        wp -= 1;

        if (wp < 0) // wrap write pointer
            wp = maxDelay - 1;

        rp = wp + delayLenSamples;
        if (rp >= maxDelay) // wrap read pointer
            rp -= maxDelay;

        readPtr = &buffer[rp];
        writePtr = &buffer[wp];
    }

    // Direct read and write pointers are made available for speed.
    // Use these with great caution! The read() and write()
    // functions are preferred.
    float* readPtr;
    float* writePtr;

private:
    enum
    {
        maxDelay = 64*8192,
    };

    float buffer[maxDelay];
    int wp = 0; // write pointer
    int rp = 0; // read pointer
    int delayLenSamples = 0; // delay length in samples

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLine)
};

#endif //DELAYLINE_H_INCLUDED
