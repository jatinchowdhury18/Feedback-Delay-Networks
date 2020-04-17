#ifndef SINEWAVE_H_INCLUDED
#define SINEWAVE_H_INCLUDED

#include "JuceHeader.h"

/**
 * Simple sine wave oscillator based on JUCE example,
 * see: https://docs.juce.com/master/tutorial_sine_synth.html
 **/
class SineWave
{
public:
    SineWave() {}

    /** Set oscillation frequency */
    void setFreq (float newFreq)
    {
        freq = newFreq;

        auto cyclesPerSamp = freq / fs;
        angleDelta = cyclesPerSamp * MathConstants<float>::twoPi;
    }

    /** Set oscillation amplitude */
    void setAmp (float newAmp)
    {
        amp = newAmp;
    }

    /** Set oscillator phase (0 - 2pi) */
    void setPhase (float newPhase)
    {
        phase = jlimit (0.0f, MathConstants<float>::twoPi, newPhase);
    }

    /** Reset internal sammple rate */
    void reset (float sampleRate)
    {
        fs = sampleRate;
        setFreq (freq);
    }

    void processBlock (float* block, const int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
        {
            block[n] = sin (phase + curAngle);
            curAngle += angleDelta;
        }

        if (curAngle > MathConstants<float>::twoPi)
            curAngle -= MathConstants<float>::twoPi;

        FloatVectorOperations::multiply (block, amp, numSamples);
    }

    /** Compute next sample */
    inline float getNextSample()
    {
        auto curSample = sin (phase + curAngle);
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
    float phase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineWave)
};

#endif // SINEWAVE_H_INCLUDED
