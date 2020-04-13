#include "LevelDetector.h"

void LevelDetector::setAttackMs (float newAtt)
{
    if (attackMs != newAtt)
    {
        attackMs = newAtt;
        a1_a = expf (-1.0f / (fs * attackMs / 1000.0f));
        b0_a = 1.0f - a1_a;
    }
}

void LevelDetector::setReleaseMs (float newRel)
{
    if (releaseMs != newRel)
    {
        releaseMs = newRel;
        a1_r = expf (-1.0f / (fs * releaseMs / 1000.0f));
        b0_r = 1.0f - a1_r;
    }
}

void LevelDetector::setType (DetectorType type)
{
    if (type == DetectorType::Peak)
        process = &LevelDetector::processPeak;
    else
        process = &LevelDetector::processRMS;
}

void LevelDetector::reset (float sampleRate)
{
    fs = sampleRate;
    levelEst = 0.0f;

    setAttackMs (attackMs);
    setReleaseMs (releaseMs);
}

void LevelDetector::processBlock (float* buffer, const int numSamples)
{
    for (int n = 0; n < numSamples; ++n)
        buffer[n] = (this->*process) (buffer[n]);
}
