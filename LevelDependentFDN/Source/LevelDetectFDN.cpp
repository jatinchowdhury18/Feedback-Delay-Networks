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
    detectBuffer.reset (new float[maxNumSamples]);
    gLowBuffer  .reset (new float[maxNumSamples]);
    gHighBuffer .reset (new float[maxNumSamples]);
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

    FloatVectorOperations::multiply (detectBuffer.get(), levelScale, maxNumSamples);
    FloatVectorOperations::add      (detectBuffer.get(), levelOffset, maxNumSamples);
    
    FloatVectorOperations::multiply (gLowBuffer.get(),  detectBuffer.get(), gLow,  maxNumSamples);
    FloatVectorOperations::multiply (gHighBuffer.get(), detectBuffer.get(), gHigh, maxNumSamples);

    FloatVectorOperations::min (gLowBuffer.get(),  gLowBuffer.get(),  1.0f, maxNumSamples);
    FloatVectorOperations::min (gHighBuffer.get(), gHighBuffer.get(), 1.0f, maxNumSamples);
}

void LevelDetectFDN::copyDetectBuffer (const float* buffer, int numSamples)
{
    FloatVectorOperations::copy (detectBuffer.get(), buffer, numSamples);
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
