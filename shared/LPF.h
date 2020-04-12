#ifndef LPF_H_INCLUDED
#define LPF_H_INCLUDED

#include "Biquad.h"

/** Second order lowpass filter */
class LPF : public Biquad
{
public:
    LPF() {}
    ~LPF() {}

    void calcCoefs (float fc, float Q)
    {
        // compute filter coefficients for 2-nd order
        // butterworth LPF with bilinear transform and
        // frequency warping.
        //
        //                1
        // H(s) = -----------------
        //         s^2 + s / Q + 1

        float wc = MathConstants<float>::twoPi * fc / fs;
        float c = 1.0f / dsp::FastMathApproximations::tan (wc / 2.0f);
        float phi = c * c;
        float K = c / Q;
        float a0 = phi + K + 1.0f;

        b[0] = 1.0f / a0;
        b[1] = 2.0f * b[0];
        b[2] = b[0];
        a[1] = 2.0f * (1.0f - phi) / a0;
        a[2] = (phi - K + 1.0f) / a0;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LPF)
};

#endif // LPF_H_INCLUDED
