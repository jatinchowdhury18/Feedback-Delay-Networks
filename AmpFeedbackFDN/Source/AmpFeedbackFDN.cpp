#include "AmpFeedbackFDN.h"

AmpFeedbackFDN::AmpFeedbackFDN (int numDelays) :
    FDN (numDelays)
{
}

void AmpFeedbackFDN::reset (float sampleRate)
{
    FDN::reset (sampleRate);

    fbDelay.reset();
    lpf.reset (sampleRate);
    sine.reset (sampleRate);
    dcBlocker.reset (sampleRate);
}

void AmpFeedbackFDN::updateParams()
{
    FDN::updateParams();

    lpf.calcCoefs (fc, 0.7071f);
}

void AmpFeedbackFDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    // smooth shelf parameters if needed
    if (shelfs[0].needsSmooth())
        shelfProcess = &ShelfFilter::processSampleSmooth;
    else
        shelfProcess = &ShelfFilter::processSample;

    for (int n = 0; n < numSamples; ++n)
    {
        fbDelay.setDelay (jmax (delayLenMs * fs / 1000.0f + sine.getNextSample(), 0.0f));

        block[n] = fbGain * fbDelay.read() + (1.0f - fbGain) * block[n];
        fbDelay.updatePtrs();

        block[n] = processSample (block[n]);
        updatePtrs();

        fbDelay.write (dcBlocker.process (lpf.process (distortion (block[n]))));
    }
}
