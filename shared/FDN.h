#ifndef FDN_H_INCLUDED
#define FDN_H_INCLUDED

#include "DelayLine.h"
#include "ShelfFilter.h"

class FDN
{
public:
    FDN (int numDelays);

    void setSize (float newSize) { size = newSize; }
    void setT60Low (float newT60Low) { t60Low = newT60Low; }
    void setT60High (float newT60High) { t60High = newT60High; }

    virtual void reset (float sampleRate);
    void updateParams();

    virtual inline float delayLineAccum (int dInd)
    {
        float accum = 0.0f;

        for (int sumInd = 0; sumInd < numDelays; sumInd++)
            accum += matrix[dInd][sumInd] * delayLines[sumInd].read();

        return accum;
    }

    inline void updatePtrs()
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
            delayLines[dInd].updatePtrs();
    }

    virtual inline float processSample (float x)
    {
        float y = 0.0f;

        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            auto accum = delayLineAccum (dInd);

            y += accum; // add to output
            accum += x; // add input to accumulator
            accum = shelfs[dInd].processSample (accum); // filter
            delayLines[dInd].write (accum);
        }

        return y;
    }

    virtual void processBlock (float* block, const int numSamples);

protected:
    const int numDelays;
    float fs = 44100.0f;
    float t60Low = 0.5f; // seconds
    float t60High = 0.5f; // seconds
    float size = 0.2f; // (0,  1])

    std::vector<int> delayLensMs; // milliseconds

    std::unique_ptr<DelayLine[]> delayLines;
    std::unique_ptr<ShelfFilter[]> shelfs;

    float matrix[12][12] = {
            {+0.63912f, +0.52380f, -0.41697f, +0.11043f, -0.12455f, -0.00443f, -0.00515f, -0.09814f, +0.00020f, -0.03819f, +0.25988f, +0.19213f, },
            {+0.53947f, +0.07753f, +0.46527f, -0.17110f, +0.14097f, -0.01164f, +0.03594f, -0.08002f, -0.03511f, -0.05676f, -0.16863f, -0.62978f, },
            {-0.38461f, +0.47142f, -0.03015f, +0.18607f, -0.21664f, -0.11385f, -0.10051f, -0.00205f, -0.13452f, +0.48722f, +0.18851f, -0.48300f, },
            {+0.11549f, -0.16923f, +0.24248f, +0.87265f, -0.00799f, +0.03538f, +0.03743f, -0.32297f, +0.10286f, +0.08233f, -0.09029f, +0.07191f, },
            {-0.10333f, +0.15643f, -0.20733f, +0.08803f, +0.75408f, +0.05174f, +0.56302f, +0.04113f, +0.06349f, +0.11754f, +0.02935f, -0.07359f, },
            {-0.00070f, -0.02606f, -0.04298f, +0.00534f, -0.19633f, +0.94148f, +0.10985f, +0.12521f, +0.13780f, +0.05288f, +0.06379f, -0.13754f, },
            {+0.00604f, -0.02103f, +0.14505f, -0.06344f, -0.52588f, -0.17968f, +0.80939f, +0.01936f, -0.07884f, -0.01637f, +0.00890f, +0.05980f, },
            {+0.09295f, +0.06716f, -0.03362f, +0.33674f, -0.04308f, -0.11748f, -0.03299f, +0.88120f, +0.03701f, -0.15751f, -0.21566f, -0.07987f, },
            {+0.00553f, +0.03571f, +0.11946f, -0.11200f, -0.05422f, -0.16669f, -0.00966f, +0.04689f, +0.94259f, +0.15459f, +0.16273f, +0.00220f, },
            {+0.03050f, +0.00691f, -0.44899f, -0.06089f, -0.14023f, -0.02089f, +0.02007f, -0.18515f, +0.15422f, +0.20397f, -0.81415f, -0.10350f, },
            {-0.29988f, +0.19520f, -0.17512f, +0.12790f, -0.07240f, -0.01997f, +0.02620f, -0.21981f, +0.16471f, -0.79869f, +0.01336f, -0.32416f, },
            {-0.17027f, +0.63184f, +0.48624f, -0.06766f, +0.06469f, +0.14788f, -0.02731f, -0.02455f, +0.00962f, -0.07780f, -0.34542f, +0.42181f, },
           };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDN)
};

#endif //FDN_H_INCLUDED
