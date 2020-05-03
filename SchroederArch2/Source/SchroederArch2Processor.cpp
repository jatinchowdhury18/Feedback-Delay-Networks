#include "SchroederArch2Processor.h"

namespace
{
    const String fbTag    = "fb";
    const String delayTag = "delay";
    const String gTag     = "gval";
    const String dTag     = "dval";
}

SchroederArch2Processor::SchroederArch2Processor()
{
    fbParam = vts.getRawParameterValue (fbTag);
    delayParam = vts.getRawParameterValue (delayTag);

    for (int i = 0; i < 4; ++i)
    {
        gParams[i] = vts.getRawParameterValue (gTag + String (i));
        dParams[i] = vts.getRawParameterValue (dTag + String (i));
    }
}

SchroederArch2Processor::~SchroederArch2Processor()
{
}

void SchroederArch2Processor::addParameters (Parameters& params)
{
    params.push_back (std::make_unique<AudioParameterFloat> (fbTag, "FB", -0.9f, 0.9f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (delayTag, "Delay", 0.0f, 5000.0f, 500.0f));

    for (int i = 0; i < 4; ++i)
    {
        auto iStr = String (i);
        params.push_back (std::make_unique<AudioParameterFloat> (gTag + iStr, "G" + iStr, -0.9f, 0.9f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat> (dTag + iStr, "D" + iStr, 0.0f, 5000.0f, 500.0f));
    }
}

void SchroederArch2Processor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        procs[ch].reset();
}

void SchroederArch2Processor::releaseResources()
{
}

void SchroederArch2Processor::updateParams()
{
    for (int ch = 0; ch < 2; ++ch)
    {
        procs[ch].setFBGain (*fbParam);
        procs[ch].setDelay ((int) *delayParam);

        for (int i = 0; i < 4; ++i)
            procs[ch].setAPParams ((int) *dParams[i], *gParams[i], i);
    }
}

void SchroederArch2Processor::processBlock (AudioBuffer<float>& buffer)
{
    updateParams();

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x =  buffer.getWritePointer (ch);
        procs[ch].processBlock (x, buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new SchroederArch2Processor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SchroederArch2Processor();
}
#endif
