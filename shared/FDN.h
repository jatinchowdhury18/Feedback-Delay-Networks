#ifndef FDN_H_INCLUDED
#define FDN_H_INCLUDED

#include "DelayLine.h"
#include "ShelfFilter.h"
#include "MixingMatrixUtils.h"
#include <numeric>

class FDN
{
public:
    FDN (int numDelays);
    ~FDN();

    void setSize (float newSize) { size = newSize; }
    void setT60Low (float newT60Low) { t60Low = newT60Low; }
    void setT60High (float newT60High) { t60High = newT60High; }

    virtual void reset (float sampleRate);
    virtual void updateParams();

    // DEPRECATED in favor of std::inner_product (keeping it around for reference)
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

    // This function has been VERY carefully optimised.
    // Be careful when making adjustments
    virtual inline float processSample (float x)
    {
        float y = 0.0f;

        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            for (int sumInd = 0; sumInd < numDelays; sumInd++)
                delayReads[sumInd] = *delayLines[sumInd].readPtr;
            auto accum = std::inner_product (matrix.matrix[dInd], matrix.matrix[dInd] + numDelays, delayReads, 0.0f);

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

    // Ideally I'd prefer smart pointers over raw pointers, but it
    // seems that smart pointers have some performance overhead that
    // becomes noticeable when doing computations like this.
    DelayLine* delayLines;
    ShelfFilter* shelfs;

    Matrix matrix;
    float* delayReads = nullptr;
    float* delayAccums = nullptr;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDN)
};

#endif //FDN_H_INCLUDED
