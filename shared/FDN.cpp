#include "FDN.h"
#include "DelayUtils.h"

FDN::FDN (int numDelays) :
    numDelays (numDelays),
    matrix (numDelays)
{
    delayLensMs = DelayUtils::generateDelayLengths (numDelays, 45, 1.1f, NextDelayType::AddOne);
    delayLines = new DelayLine[numDelays];
    shelfs = new ShelfFilter[numDelays];

    MixingMatrixUtils::myMatrix (matrix);

    delayReads = new float[numDelays];
}

FDN::~FDN()
{
    // NO MEMORY LEAKS
    delete[] delayReads;
    delete[] delayAccums;

    delete[] delayLines;
    delete[] shelfs;
}

void FDN::reset (float sampleRate)
{
    fs = sampleRate;

    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        delayLines[dInd].reset();
        shelfs[dInd].reset (fs);
    }        
}

void FDN::updateParams()
{
    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        auto curDelayLen = (int) (((float) delayLensMs[dInd] / 1000.0f) * size * fs);
        delayLines[dInd].setDelay (curDelayLen);

        auto gLow = DelayUtils::calcGainForT60 (curDelayLen, fs, t60Low);
        auto gHigh = DelayUtils::calcGainForT60 (curDelayLen, fs, t60High);

        shelfs[dInd].setFreq (2500.0f);
        shelfs[dInd].setLowGain (gLow);
        shelfs[dInd].setHighGain (gHigh);
    }
}

void FDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    for (int n = 0; n < numSamples; ++n)
    {
        block[n] = processSample (block[n]);
        updatePtrs();
    }
}
