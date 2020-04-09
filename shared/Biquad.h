#ifndef BIQUAD_H_INCLUDED
#define BIQUAD_H_INCLUDED

#include "JuceHeader.h"

class Biquad
{
public:
    Biquad() {}
    virtual ~Biquad() {}

    virtual void reset (float sampleRate)
    {
        fs = sampleRate;

        for (int n = 0; n < order+1; ++n)
            z[n] = 0.0f;
    }

    virtual inline float process (float x)
    {
        // process input sample, direct form II transposed
        float y = z[1] + x*b[0];
        z[1] = z[2] + x*b[1] - y*a[1];
        z[2] = x*b[2] - y*a[2];

        return y;
    }

protected:
    float fs = 44100.0f;
    float a[3] = { 1.0f, 0.0f, 0.0f };
    float b[3] = { 1.0f, 0.0f, 0.0f };
    float z[3] = { 1.0f, 0.0f, 0.0f };

private:
    const int order = 2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Biquad)
};

#endif // BIQUAD_H_INCLUDED
