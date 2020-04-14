#include "ShelfFilter.h"

ShelfFilter::ShelfFilter()
{
    lowGainSmooth.reset (smoothSteps);
    highGainSmooth.reset (smoothSteps);
    fcSmooth.reset (smoothSteps);
}

void ShelfFilter::setLowGain (float newLowGain)
{
    if (newLowGain != lowGainSmooth.getTargetValue())
        lowGainSmooth.setTargetValue (newLowGain);
}

void ShelfFilter::setLowGainForce (float newLowGain)
{
    lowGainSmooth.setCurrentAndTargetValue (newLowGain);
}

void ShelfFilter::setHighGain (float newHighGain)
{
    if (newHighGain != highGainSmooth.getTargetValue())
        highGainSmooth.setTargetValue (newHighGain);
}

void ShelfFilter::setHighGainForce (float newHighGain)
{
    highGainSmooth.setCurrentAndTargetValue (newHighGain);
}

void ShelfFilter::setFreq (float newFreq)
{
    if (newFreq != fcSmooth.getTargetValue())
        fcSmooth.setTargetValue (newFreq);
}

void ShelfFilter::reset (float sampleRate)
{
    fs = sampleRate;

    z[0] = 0.0f; z[1] = 0.0f;

    calcCoefs (lowGainSmooth.skip (smoothSteps), highGainSmooth.skip (smoothSteps), fcSmooth.skip (smoothSteps));
}

void ShelfFilter::calcCoefs (float lowGain, float highGain, float fc)
{
    // reduce to simple gain element
    if (lowGain == highGain)
    {
        b[0] = lowGain; b[1] = 0.0f;
        a[0] = 1.0f; a[1] = 0.0f;
        return;
    }

    auto wc = MathConstants<float>::twoPi * fc;
    auto p = sqrt (wc*wc * (highGain*highGain - lowGain*highGain) / (lowGain*highGain - lowGain*lowGain));
    auto K = p / tan (p / (2.0f * fs));

    auto b0 = highGain / p;
    auto b1 = lowGain;
    auto a0 = 1.0f / p;
    auto a1 = 1.0f;

    auto a0_z = a0*K + a1;

    b[0] = (b0*K + b1) / a0_z;
    b[1] = (-b0*K + b1) / a0_z;
    a[0] = 1.0f;
    a[1] = (-a0*K + a1) / a0_z;
}
