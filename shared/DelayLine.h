#include "JuceHeader.h"

/**
    A collection of structs to pass as the template argument when setting the
    interpolation type for the DelayLine class.
*/
namespace DelayLineInterpolationTypes
{
    /**
        No interpolation between successive samples in the delay line will be
        performed. This is useful when the delay is a constant integer or to
        create lo-fi audio effects.
    */
    struct None {};

    /**
        Successive samples in the delay line will be linearly interpolated. This
        type of interpolation has a low compuational cost where the delay can be
        modulated in real time, but it also introduces a low-pass filtering effect
        into your audio signal.
    */
    struct Linear {};

    /**
        Successive samples in the delay line will be interpolated using a 3rd order
        Lagrange interpolator. This method incurs more computational overhead than
        linear interpolation but reduces the low-pass filtering effect whilst
        remaining amenable to real time delay modulation.
    */
    struct Lagrange3rd {};

    /**
        Successive samples in the delay line will be interpolated using 1st order
        Thiran interpolation. This method is very efficient, and features a flat
        amplitude frequency response in exchange for less accuracy in the phase
        response. This interpolation method is stateful so is unsuitable for
        applications requiring fast delay modulation.
    */
    struct Thiran {};
}

/**
    A delay line processor featuring several algorithms for the fractional delay
    calculation, and sample-by-sample processing.

    Note: If you intend to change the delay in real time, you may want to smooth
    changes to the delay systematically using either a ramp or a low-pass filter.

    Adapted from JUCE6 DSP module
*/
template <typename SampleType, typename InterpolationType = DelayLineInterpolationTypes::Linear>
class DelayLine
{
public:
    /** Default constructor. */
    DelayLine();

    /** Sets the delay in samples. */
    void setDelay (SampleType newDelayInSamples);

    /** Returns the current delay in samples. */
    SampleType getDelay() const;

    /** Resets the internal state variables of the processor. */
    virtual void reset();

    /** Writes a single sample into the delay line. */
    virtual inline void write (SampleType data) noexcept
    {
        buffer[wp] = data;
    }

    virtual inline void write (SampleType data, int idx) noexcept
    {
        buffer[idx] = data;
    }

    /** Reads a single sample from the delay line. */
    virtual inline SampleType read() noexcept
    {
        return interpolateSample (rp);
    }

    virtual inline SampleType read (int idx) noexcept
    {
        return interpolateSample (idx);
    }

    /** Update delay line read pointer */
    virtual inline void updatePtrs() noexcept
    {
        wp -= 1;

        if (wp < 0) // wrap write pointer
            wp = maxDelay - 1;

        rp = wp + (int) delay;
        if (rp >= maxDelay) // wrap read pointer
            rp -= maxDelay;
    }

private:
    template <typename T = InterpolationType>
    inline typename std::enable_if <std::is_same <T, DelayLineInterpolationTypes::None>::value, SampleType>::type
    interpolateSample (int idx) const
    {
        return buffer[idx];
    }

    template <typename T = InterpolationType>
    inline typename std::enable_if <std::is_same <T, DelayLineInterpolationTypes::Linear>::value, SampleType>::type
    interpolateSample (int idx) const
    {
        auto index1 = idx;
        auto index2 = index1 + 1;

        if (index2 >= maxDelay)
        {
            index1 %= maxDelay;
            index2 %= maxDelay;
        }

        auto value1 = buffer[index1];
        auto value2 = buffer[index2];

        return value1 + delayFrac * (value2 - value1);
    }

    template <typename T = InterpolationType>
    inline typename std::enable_if <std::is_same <T, DelayLineInterpolationTypes::Lagrange3rd>::value, SampleType>::type
    interpolateSample (int idx) const
    {
        auto index1 = idx;
        auto index2 = index1 + 1;
        auto index3 = index2 + 1;
        auto index4 = index3 + 1;

        if (index4 >= maxDelay)
        {
            index1 %= maxDelay;
            index2 %= maxDelay;
            index3 %= maxDelay;
            index4 %= maxDelay;
        }

        auto value1 = buffer[index1];
        auto value2 = buffer[index2];
        auto value3 = buffer[index3];
        auto value4 = buffer[index4];

        auto d1 = delayFrac - 1.f;
        auto d2 = delayFrac - 2.f;
        auto d3 = delayFrac - 3.f;

        auto c1 = -d1 * d2 * d3 / 6.f;
        auto c2 = d2 * d3 * 0.5f;
        auto c3 = -d1 * d3 * 0.5f;
        auto c4 = d1 * d2 / 6.f;

        return value1 * c1 + delayFrac * (value2 * c2 + value3 * c3 + value4 * c4);
    }

    template <typename T = InterpolationType>
    inline typename std::enable_if <std::is_same <T, DelayLineInterpolationTypes::Thiran>::value, SampleType>::type
    interpolateSample (int idx)
    {
        auto index1 = idx;
        auto index2 = index1 + 1;

        if (index2 >= maxDelay)
        {
            index1 %= maxDelay;
            index2 %= maxDelay;
        }

        auto value1 = buffer[index1];
        auto value2 = buffer[index2];

        auto output = delayFrac == 0.0f ? value1 : value2 + alpha * (value1 - z);
        z = output;

        return output;
    }
    
    //==============================================================================
    enum
    {
        maxDelay = 64*8192,
    };

    SampleType buffer[maxDelay];
    int wp = 0; // write pointer
    int rp = 0; // read pointer

    SampleType delay = 0.0, delayFrac = 0.0;
    SampleType alpha = 0.0;
    SampleType z = 0.0f;
};
