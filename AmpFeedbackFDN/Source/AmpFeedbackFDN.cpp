#include "AmpFeedbackFDN.h"

AmpFeedbackFDN::AmpFeedbackFDN (int numDelays) :
    FDN (numDelays)
{
    MixingMatrixUtils::myMatrix (matrix);
}

void AmpFeedbackFDN::reset (float sampleRate)
{
    FDN::reset (sampleRate);

    fbDelay.reset (sampleRate);
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

    for (int n = 0; n < numSamples; ++n)
    {
        fbDelay.setDelay (jmax (delayLenMs + sine.getNextSample(), 0.0f));

        block[n] = fbGain * fbDelay.read() + (1.0f - fbGain) * block[n];
        fbDelay.updatePtrs();

        block[n] = processSample (block[n]);
        updatePtrs();

        fbDelay.write (dcBlocker.process (lpf.process (distortion (block[n]))));
    }
}
