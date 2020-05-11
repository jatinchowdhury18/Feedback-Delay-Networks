#ifndef FDN_H_INCLUDED
#define FDN_H_INCLUDED

#include "DelayLine.h"
#include "ShelfFilter.h"
#include "MixingMatrixUtils.h"
#include <numeric>

/**
 * Basic Feedback Delay Network (FDN).
 * For more information see: https://ccrma.stanford.edu/~jos/pasp/Feedback_Delay_Networks_FDN.html
 * */
class FDN
{
public:
    FDN (int numDelays);
    ~FDN();

    /** Scale the delay lines of the FDN to approximate room size */
    void setSize (float newSize) { size = newSize; }

    /** Set low frequency decay time */
    void setT60Low (float newT60Low) { t60Low = newT60Low; }

    /** Set high frequency decay time */
    void setT60High (float newT60High) { t60High = newT60High; }

    /** Reset FDN delay lines and filters */
    virtual void reset (float sampleRate);

    /**
     * Update FDN parameters, and recompute intermediate variables.
     * It is recommended to call this function at the beginning
     * of every buffer being processed.
     * */
    virtual void updateParams();

    // DEPRECATED in favor of std::inner_product (keeping it around for reference)
    virtual inline float delayLineAccum (int dInd)
    {
        float accum = 0.0f;

        for (int sumInd = 0; sumInd < numDelays; sumInd++)
            accum += matrix.matrix[dInd][sumInd] * delayLines[sumInd].read();

        return accum;
    }

    /**
     * Update FDN delay line ptrs.
     * This function should be called every time
     * after a sample has been processed.
     * */
    virtual inline void updatePtrs()
    {
        for (int dInd = 0; dInd < numDelays; ++dInd)
            delayLines[dInd].updatePtrs();
    }

    /**
     * Process a single sample through the FDN.
     * 
     * N.B: This function has been VERY carefully optimised.
     * Be careful when making adjustments, or overriding.
     * */
    virtual inline float processSample (float x)
    {
        float y = 0.0f;
        // accumulate values from delay lines
        for (int sumInd = 0; sumInd < numDelays; sumInd++)
            delayReads[sumInd] = delayLines[sumInd].read();

        for (int dInd = 0; dInd < numDelays; ++dInd)
        {
            // multiply by mixing matrix
            auto accum = std::inner_product (matrix.matrix[dInd], matrix.matrix[dInd] + numDelays, delayReads, 0.0f);

            y += accum; // add to output
            accum += x; // add input to accumulator
            accum = (shelfs[dInd].*shelfProcess) (accum); // filter
            delayLines[dInd].write (accum);
        }

        return y;
    }

    /** Process a block of samples */
    virtual void processBlock (float* block, const int numSamples);

    Matrix& getMatrix() { return matrix; }

protected:
    const int numDelays;
    float fs = 44100.0f;
    float t60Low = 0.5f; // seconds
    float t60High = 0.5f; // seconds
    float size = 0.2f; // (0,  1])

    std::vector<int> delayLensMs; // milliseconds

    // Ideally smart pointers are preferred over raw pointers,
    // but it seems that smart pointers have some performance
    // overhead that becomes noticeable when doing these computations.
    DelayLine<float>* delayLines;
    ShelfFilter* shelfs;

    Matrix matrix;
    float* delayReads = nullptr;
    float* delayAccums = nullptr;

    // Shelf process function pointer
    float (ShelfFilter::*shelfProcess) (float) = &ShelfFilter::processSample;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDN)
};

#endif //FDN_H_INCLUDED
