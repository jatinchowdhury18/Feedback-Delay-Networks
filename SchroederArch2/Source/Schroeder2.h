#ifndef SCHROEDER2_H_INCLUDED
#define SCHROEDER2_H_INCLUDED

#include "AllpassDelay.h"

/**
 * Reverb algorithm based on one of Schroeder's
 * (Not the famous one).
 * 
 * See: https://valhalladsp.com/wp-content/uploads/2010/05/picture-102.png
 **/
class Schroeder2
{
public:
    Schroeder2() {}

    void reset()
    {
        for (int i = 0; i < numAPs; ++i)
            aps[i].reset();

        delay.reset();
    }

    void setFBGain (float gain)
    {
        fbGain = gain;
    }

    void setDelay (int delaySamp)
    {
        delay.setDelay (delaySamp);
    }

    void setAPParams (int delaySamp, float gVal, int apIdx)
    {
        jassert (apIdx < numAPs);
        aps[apIdx].setDelay (delaySamp);
        aps[apIdx].setG (gVal);
    }

    inline float processSample (float x)
    {
        auto z = aps[3].read();

        aps[3].write (aps[2].read());
        aps[2].write (aps[1].read());
        aps[1].write (aps[0].read());
        aps[0].write (delay.read());
        delay.write (x + fbGain * z);

        updatePtrs();

        return z * (1.0f - fbGain) * (1.0f - fbGain) - fbGain * x;
    }

    inline void updatePtrs()
    {
        delay.updatePtrs();
        aps[0].updatePtrs();
        aps[1].updatePtrs();
        aps[2].updatePtrs();
        aps[3].updatePtrs();
    }

    void processBlock (float* buffer, const int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample (buffer[n]);
    }

private:
    enum
    {
        numAPs = 4,
    };

    AllpassDelay aps[numAPs];
    DelayLine delay;

    float fbGain = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Schroeder2)
};

#endif // SCHROEDER2_H_INCLUDED
