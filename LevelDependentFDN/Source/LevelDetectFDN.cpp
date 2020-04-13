#include "LevelDetectFDN.h"
#include "DelayUtils.h"

LevelDetectFDN::LevelDetectFDN (int numDelays) :
    FDN (numDelays)
{
    
}

void LevelDetectFDN::prepare (float sampleRate, int samplesPerBlock)
{
    FDN::reset (sampleRate);

    maxNumSamples = samplesPerBlock;
    detectBuffer = new float[maxNumSamples];
    gLowBuffer   = new float[maxNumSamples];
    gHighBuffer  = new float[maxNumSamples];
}

void LevelDetectFDN::releaseResources()
{
    delete[] detectBuffer;
    delete[] gLowBuffer;
    delete[] gHighBuffer;
}

void LevelDetectFDN::updateParams()
{
    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        // compute delay line lengths
        auto curDelayLen = (int) (((float) delayLensMs[dInd] / 1000.0f) * size * fs);
        delayLines[dInd].setDelay (curDelayLen);

        // compute gains for desired T60s
        gLow = DelayUtils::calcGainForT60 (curDelayLen, fs, t60Low);
        gHigh = DelayUtils::calcGainForT60 (curDelayLen, fs, t60High);

        // Set shelf filter parameters
        shelfs[dInd].setFreq (2500.0f);
    }

    FloatVectorOperations::multiply (detectBuffer, levelScale, maxNumSamples);
    FloatVectorOperations::add      (detectBuffer, levelOffset, maxNumSamples);
    
    FloatVectorOperations::multiply (gLowBuffer,  detectBuffer, gLow,  maxNumSamples);
    FloatVectorOperations::multiply (gHighBuffer, detectBuffer, gHigh, maxNumSamples);
}

void LevelDetectFDN::copyDetectBuffer (const float* buffer, int numSamples)
{
    FloatVectorOperations::copy (detectBuffer, buffer, numSamples);
}

void LevelDetectFDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    // always smooth shelf parameters
    shelfProcess = &ShelfFilter::processSampleSmooth;

    for (int n = 0; n < numSamples; ++n)
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            shelfs[dInd].setLowGainForce  (gLowBuffer[n]);
            shelfs[dInd].setHighGainForce (gHighBuffer[n]);
        }

        block[n] = processSample (block[n]);
        updatePtrs();
    }
}
