#ifndef FBPROC_H_INCLUDED
#define FBPROC_H_INCLUDED

#include "SmoothAllpassDelay.h"

class FBProc
{
public:
    FBProc()
    {
        osc.initialise ([] (float x) { return std::sin (x); });
    }

    void setFB (float fbGain) { gain = fbGain; }
    void setDelay (float delayMs) { delayLenMs = delayMs; }
    void setOscGain (float newOscGain) { oscGain = newOscGain; }
    void setOscFreq (float freq) { osc.setFrequency (freq); }
    void setDrive (float dGain) { driveGain = dGain; }
    void setSmear (float smear) { delay.setG (smear); }

    void reset (float sampleRate)
    {
        delay.reset (sampleRate);
        osc.prepare ({ sampleRate, 512, 2 });
    }

    inline float saturate (float x)
    {
        return dsp::FastMathApproximations::tanh (x* driveGain) * 0.9f;
    }

    inline float processSample (float x)
    {
        auto y = delay.read();
        delay.write (x + saturate (y * gain));
        delay.updatePtrs();

        return y;
    }

    void process (float* buffer, const int numSamples)
    {
        auto curOscGain = jmin (oscGain, delayLenMs / 2);

        for (int n = 0; n < numSamples; ++n)
        {
            delay.setDelay (delayLenMs + curOscGain * osc.processSample (0.0f));
            buffer[n] = processSample (buffer[n]);
        }
    }

private:
    SmoothAllpassDelay delay;
    float gain = 0.0f;
    float oscGain = 0.0f;
    float delayLenMs = 1.0f;
    float driveGain = 1.0f;

    dsp::Oscillator<float> osc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FBProc)
};

#endif // FBPROC_H_INCLUDED
