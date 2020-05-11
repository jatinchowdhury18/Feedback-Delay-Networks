#include "DelayLine.h"

template <typename SampleType, typename InterpolationType>
DelayLine<SampleType, InterpolationType>::DelayLine()
{
}

template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::setDelay (SampleType newDelayInSamples)
{
    auto upperLimit = (SampleType) (maxDelay - 1);
    jassert (isPositiveAndNotGreaterThan (newDelayInSamples, upperLimit));

    delay     = jlimit ((SampleType) 0, upperLimit, newDelayInSamples);
    delayFrac = delay - roundToInt (delay);
    alpha = (1 - delayFrac) / (1 + delayFrac);
}

template <typename SampleType, typename InterpolationType>
SampleType DelayLine<SampleType, InterpolationType>::getDelay() const
{
    return delay;
}

template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::reset()
{
    FloatVectorOperations::fill (buffer, 0.0f, maxDelay);
    z = 0.0f;
}

template class DelayLine<float,  DelayLineInterpolationTypes::None>;
template class DelayLine<double, DelayLineInterpolationTypes::None>;
template class DelayLine<float,  DelayLineInterpolationTypes::Linear>;
template class DelayLine<double, DelayLineInterpolationTypes::Linear>;
template class DelayLine<float,  DelayLineInterpolationTypes::Lagrange3rd>;
template class DelayLine<double, DelayLineInterpolationTypes::Lagrange3rd>;
template class DelayLine<float,  DelayLineInterpolationTypes::Thiran>;
template class DelayLine<double, DelayLineInterpolationTypes::Thiran>;
