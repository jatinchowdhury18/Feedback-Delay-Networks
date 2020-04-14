#ifndef LEVELDETECTOR_H_INCLUDED
#define LEVELDETECTOR_H_INCLUDED

#include "JuceHeader.h"

enum DetectorType
{
    Peak,
    RMS,
};

class LevelDetector
{
public:
    LevelDetector() {}

    void setAttackMs  (float newAtt);
    void setReleaseMs (float newRel);
    void setType (DetectorType type);

    void reset (float sampleRate);
    void processBlock (float* buffer, const int numSamples);

    void setLevelEst (float levelEstimate) { levelEst = levelEstimate; }
    float getLevelEst() const noexcept { return levelEst; }

    // process function pointer
    float (LevelDetector::*process) (float) = &LevelDetector::processPeak;

private:
    inline float processPeak (float x)
    {
        auto input = fabs (x);

        if (input > levelEst)
            levelEst += b0_a * (input - levelEst);
        else
            levelEst += b0_r * (input - levelEst);
        
        return levelEst;
    }

    inline float processRMS (float x)
    {
        auto input = x*x;
        auto outputSquare = 0.0f;

        if (input > levelEst)
            outputSquare = a1_a * levelEst + b0_a * input;
        else
            outputSquare = a1_r * levelEst + b0_r * input;

        levelEst = outputSquare;
        return sqrt (outputSquare);
    }

    // Attack coefs
    float attackMs = 1.0f;
    float a1_a = 0.0f;
    float b0_a = 1.0f;

    // Release coefs
    float releaseMs = 50.0f;
    float a1_r = 0.0f;
    float b0_r = 1.0f;

    float levelEst = 0.0f;
    float fs = 48000.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelDetector)
};

#endif // LEVELDETECTOR_H_INCLUDED
