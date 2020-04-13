#ifndef LEVELDETECTFDN_H_INCLUDED
#define LEVELDETECTFDN_H_INCLUDED

#include "FDN.h"

class LevelDetectFDN : public FDN
{
public:
    LevelDetectFDN (int numDelays);

    void setOffset (float newOff)   { levelOffset = newOff; }
    void setScale  (float newScale) { levelScale  = newScale; }

    void prepare (float sampleRate, int samplesPerBlock);
    void releaseResources();

    void updateParams() override;
    void copyDetectBuffer (const float* buffer, int numSamples);
    void processBlock (float* block, const int numSamples) override;

private:
    int maxNumSamples = 0;
    float* detectBuffer = nullptr;
    float* gLowBuffer   = nullptr;
    float* gHighBuffer  = nullptr;

    float gLow = 0.0f;
    float gHigh = 0.0f;

    float levelOffset = 0.0f;
    float levelScale  = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelDetectFDN)
};

#endif // LEVELDETECTFDN_H_INCLUDED
