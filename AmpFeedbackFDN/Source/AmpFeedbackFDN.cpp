#include "AmpFeedbackFDN.h"

AmpFeedbackFDN::AmpFeedbackFDN (int numDelays) :
    FDN (numDelays)
{
    MixingMatrixUtils::myMatrix (matrix);
}

void AmpFeedbackFDN::reset (float sampleRate)
{
    FDN::reset (sampleRate);

    fbDelay.reset();
    lpf.reset (sampleRate);
}

void AmpFeedbackFDN::updateParams()
{
    FDN::updateParams();

    auto delayLenSamp = (int) ((delayLenMs / 1000.0f) * fs);
    fbDelay.setDelay (delayLenSamp);

    lpf.calcCoefs (fc, 0.7071f);
}

void AmpFeedbackFDN::processBlock (float* block, const int numSamples)
{
    updateParams();

    for (int n = 0; n < numSamples; ++n)
    {
        block[n] += fbGain * fbDelay.read();
        fbDelay.updatePtrs();

        block[n] = processSample (block[n]);
        updatePtrs();

        fbDelay.write (lpf.process (distortion (block[n])));
    }
}
