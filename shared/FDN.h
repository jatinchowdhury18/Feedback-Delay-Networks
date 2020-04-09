#ifndef FDN_H_INCLUDED
#define FDN_H_INCLUDED

#include "DelayLine.h"
#include "ShelfFilter.h"
#include "MixingMatrixUtils.h"

class FDN
{
public:
    FDN (int numDelays);

    void setSize (float newSize) { size = newSize; }
    void setT60Low (float newT60Low) { t60Low = newT60Low; }
    void setT60High (float newT60High) { t60High = newT60High; }

    virtual void reset (float sampleRate);
    virtual void updateParams();

    virtual inline float delayLineAccum (int dInd)
    {
        float accum = 0.0f;

        for (int sumInd = 0; sumInd < numDelays; sumInd++)
            accum += matrix.matrix[dInd][sumInd] * delayLines[sumInd].read();

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

        if (x != 0.0f)
            y = 0.0f;

        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            auto accum = delayLineAccum (dInd);
            if (accum != 0.0f)
                y = y;

            y += accum; // add to output
            accum += x; // add input to accumulator
            accum = shelfs[dInd].processSample (accum); // filter
            delayLines[dInd].write (accum);
        }

        return y;
    }

    virtual void processBlock (float* block, const int numSamples);

    Matrix& getMatrix() { return matrix; }

protected:
    const int numDelays;
    float fs = 44100.0f;
    float t60Low = 0.5f; // seconds
    float t60High = 0.5f; // seconds
    float size = 0.2f; // (0,  1])

    std::vector<int> delayLensMs; // milliseconds

    std::unique_ptr<DelayLine[]> delayLines;
    std::unique_ptr<ShelfFilter[]> shelfs;

    Matrix matrix;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDN)
};

#endif //FDN_H_INCLUDED
