#ifndef SINEWAVE_H_INCLUDED
#define SINEWAVE_H_INCLUDED

#include "JuceHeader.h"

class SineWave
{
public:
    SineWave() {}

    void setFreq (float newFreq)
    {
        freq = newFreq;

        auto cyclesPerSamp = freq / fs;
        angleDelta = cyclesPerSamp * MathConstants<float>::twoPi;
    }

    void setAmp (float newAmp)
    {
        amp = newAmp;
    }

    void reset (float sampleRate)
    {
        fs = sampleRate;
        setFreq (freq);
    }

    inline float getNextSample()
    {
        auto curSample = sin (curAngle);
        curAngle += angleDelta;

        if (curAngle > MathConstants<float>::twoPi)
            curAngle -= MathConstants<float>::twoPi;
        
        return amp * curSample;
    }

private:
    float curAngle = 0.0f;
    float angleDelta = 0.0f;

    float fs = 44100.0f;
    float amp = 1.0f;
    float freq = 100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineWave)
};

#endif // SINEWAVE_H_INCLUDED
