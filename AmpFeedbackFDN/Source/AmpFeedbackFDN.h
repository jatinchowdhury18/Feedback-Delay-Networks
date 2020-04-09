#ifndef AMPFEEDBACKFDN_H_INCLUDED
#define AMPFEEDBACKFDN_H_INCLUDED

#include "FDN.h"
#include "LPF.h"

class AmpFeedbackFDN : public FDN
{
public:
    AmpFeedbackFDN (int numDelays);
    virtual ~AmpFeedbackFDN() {}

    void setFBTime (float lenMs) { delayLenMs = lenMs; }
    void setDrive (float newDrive) { drive = newDrive; }
    void setFBGain (float newGain) { fbGain = newGain; }
    void setCutoff (float newFc)   { fc = newFc; }

    void reset (float sampleRate) override;
    void updateParams() override;

    void processBlock (float* block, const int numSamples) override;

    inline float distortion (float x)
    {
        return tanh (drive * x) / drive;
    }

private:
    float drive = 1.0f;
    float fbGain = 0.0f;
    float delayLenMs = 1.0f;
    float fc = 1000.0f;

    DelayLine fbDelay;
    LPF lpf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpFeedbackFDN)
};

#endif // AMPFEEDBACKFDN_H_INCLUDED
