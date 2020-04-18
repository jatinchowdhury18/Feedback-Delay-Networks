#include "ChorusFDN.h"
#include "DelayUtils.h"

ChorusFDN::ChorusFDN (int numDelays) :
    FDN (numDelays)
{
    sDelayLines = new SmoothDelay[numDelays];
    sine = new SineWave[numDelays];
}

ChorusFDN::~ChorusFDN()
{
    delete[] sDelayLines;
    delete[] sine;
}

void ChorusFDN::prepare (float sampleRate, int samplesPerBlock)
{
    FDN::reset (sampleRate);

    delayMsBuffer.setSize (numDelays, samplesPerBlock);

    for (int dInd = 0; dInd < numDelays; ++dInd)
        sDelayLines[dInd].reset (sampleRate);
}

void ChorusFDN::updateParams()
{
    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        // compute delay line lengths
        auto curDelayLenSamples = (int) (((float) delayLensMs[dInd] / 1000.0f) * size * fs);

        // compute gains for desired T60s
        auto gLow = DelayUtils::calcGainForT60  (curDelayLenSamples, fs, t60Low);
        auto gHigh = DelayUtils::calcGainForT60 (curDelayLenSamples, fs, t60High);

        // Set shelf filter parameters
        shelfs[dInd].setFreq (2500.0f);
        shelfs[dInd].setLowGain (gLow);
        shelfs[dInd].setHighGain (gHigh);

        sine[dInd].setPhase (phase * MathConstants<float>::twoPi * ((float) dInd / (float) numDelays));
    }
}

void ChorusFDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        auto delays = delayMsBuffer.getWritePointer (dInd);
        sine[dInd].processBlock (delays, numSamples);
        FloatVectorOperations::add (delays, (float) delayLensMs[dInd], numSamples);
        FloatVectorOperations::max (delays, delays, 0.0f, numSamples);
    }

    // smooth shelf parameters if needed
    if (shelfs[0].needsSmooth())
        shelfProcess = &ShelfFilter::processSampleSmooth;
    else
        shelfProcess = &ShelfFilter::processSample;

    for (int n = 0; n < numSamples; ++n)
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
            sDelayLines[dInd].setDelay (delayMsBuffer.getSample (dInd, n));

        block[n] = processSample (block[n]);
        updatePtrs();
    }
}
