#ifndef CHORUSFDN_H_INCLUDED
#define CHORUSFDN_H_INCLUDED

#include "FDN.h"
#include "SineWave.h"

class ChorusFDN : public FDN
{
public:
    ChorusFDN (int numDelays);
    ~ChorusFDN();

    void setOscFreq (float newFreq)  { for (int dInd = 0; dInd < numDelays; ++dInd) sine[dInd].setFreq (newFreq); }
    void setOscAmp  (float newAmp)   { for (int dInd = 0; dInd < numDelays; ++dInd) sine[dInd].setAmp  (newAmp); }
    void setPhase   (float newPhase) { phase = newPhase; }

    void prepare (float sampleRate, int samplesPerBlock);
    void updateParams() override;

    void processBlock (float* block, const int numSamples) override;

    inline void updatePtrs() override
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
            sDelayLines[dInd].updatePtrs();
    }

    inline float processSample (float x) override
    {
        float y = 0.0f;
        // accumulate values from delay lines
        for (int sumInd = 0; sumInd < numDelays; sumInd++)
            delayReads[sumInd] = sDelayLines[sumInd].read();

        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            // multiply by mixing matrix
            auto accum = std::inner_product (matrix.matrix[dInd], matrix.matrix[dInd] + numDelays, delayReads, 0.0f);

            y += accum; // add to output
            accum += x; // add input to accumulator
            accum = (shelfs[dInd].*shelfProcess) (accum); // filter
            sDelayLines[dInd].write (accum);
        }

        return y;
    }

private:
    DelayLine<float, DelayLineInterpolationTypes::Lagrange3rd>* sDelayLines;

    SineWave* sine;
    AudioBuffer<float> delayMsBuffer;
    float phase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusFDN)
};

#endif // CHORUSFDN_H_INCLUDED
