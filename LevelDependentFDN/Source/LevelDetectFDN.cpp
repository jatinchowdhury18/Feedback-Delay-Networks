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
    gLowBuffer.setSize (numDelays, samplesPerBlock);
    gHighBuffer.setSize (numDelays, samplesPerBlock);
}

void LevelDetectFDN::updateParams()
{
    FloatVectorOperations::multiply (detectBuffer.get(), levelScale, maxNumSamples);
    FloatVectorOperations::add      (detectBuffer.get(), levelOffset, maxNumSamples);

    for (int dInd = 0; dInd < numDelays; ++dInd)
    {
        // compute delay line lengths
        auto curDelayLen = (((float) delayLensMs[dInd] / 1000.0f) * size * fs);
        delayLines[dInd].setDelay (curDelayLen);

        // compute gains for desired T60s
        auto gLow = DelayUtils::calcGainForT60 (curDelayLen, fs, t60Low);
        auto gHigh = DelayUtils::calcGainForT60 (curDelayLen, fs, t60High);

        // Set shelf filter parameters
        shelfs[dInd].setFreq (2500.0f);

        FloatVectorOperations::multiply (gLowBuffer.getWritePointer (dInd),  detectBuffer.get(), gLow,  maxNumSamples);
        FloatVectorOperations::multiply (gHighBuffer.getWritePointer (dInd), detectBuffer.get(), gHigh, maxNumSamples);

        FloatVectorOperations::min (gLowBuffer.getWritePointer (dInd),  gLowBuffer.getWritePointer (dInd),  1.0f, maxNumSamples);
        FloatVectorOperations::min (gHighBuffer.getWritePointer (dInd), gHighBuffer.getWritePointer (dInd), 1.0f, maxNumSamples);
    }
}

void LevelDetectFDN::copyDetectBuffer (const float* buffer, int numSamples)
{
    FloatVectorOperations::copy (detectBuffer.get(), buffer, numSamples);
}

void LevelDetectFDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    auto** gLowBufferPtrs  = gLowBuffer.getArrayOfReadPointers();
    auto** gHighBufferPtrs = gHighBuffer.getArrayOfReadPointers();

    // always smooth shelf parameters
    shelfProcess = &ShelfFilter::processSampleSmooth;

    for (int n = 0; n < numSamples; ++n)
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            shelfs[dInd].setLowGainForce  (gLowBufferPtrs[dInd][n]);
            shelfs[dInd].setHighGainForce (gHighBufferPtrs[dInd][n]);
        }

        block[n] = processSample (block[n]);
        updatePtrs();
    }
}
