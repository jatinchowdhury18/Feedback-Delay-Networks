#ifndef SHELFFILTER_H_INCLUDED
#define SHELFFILTER_H_INCLUDED

#include "JuceHeader.h"

/**
 * First order shelving filter.
 * For more information see: https://ccrma.stanford.edu/~jos/filters/Low_High_Shelf_Filters.html
 * */
class ShelfFilter
{
public:
    ShelfFilter();

    /** Set gain at DC */
    void setLowGain (float newLowGain);

    /** Set gain at inifitie frequency*/
    void setHighGain (float newHighGain);
    
    /** Set transition frequency */
    void setFreq (float newFreq);

    /** Reset filter state */
    void reset (float sampleRate);

    /** Compute filter coefficients */
    void calcCoefs (float lowGain, float highGain, float fc);

    inline float processSample (float x)
    {
        // Smooth parameter changes
        if (lowGainSmooth.isSmoothing() || highGainSmooth.isSmoothing() || fcSmooth.isSmoothing())
            calcCoefs (lowGainSmooth.getNextValue(), highGainSmooth.getNextValue(), fcSmooth.getNextValue());

        float y = z[1] + x * b[0];
        z[1] = x * b[1] - y * a[1];

        return y;
    }

private:
    float b[2] = { 1.0f, 0.0f };
    float a[2] = { 1.0f, 0.0f };
    float z[2] = { 1.0f, 0.0f };

    float fs = 44100.0f;

    const int smoothSteps = 200;
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> lowGainSmooth = 1.0f;
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> highGainSmooth = 1.0f;
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> fcSmooth = 1000.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShelfFilter)
};

#endif //SHELFFILTER_H_INCLUDED
