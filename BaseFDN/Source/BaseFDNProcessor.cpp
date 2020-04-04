#include "BaseFDNProcessor.h"

namespace
{
    const String sizeTag      = "size";
    const String t60LowTag    = "t60Low";
    const String t60HighTag   = "t60High";
    const String dryWetTag    = "dryWet";
    const String preDelayTag  = "preDelay";
}

BaseFDNProcessor::BaseFDNProcessor()
{
    sizeParam        = vts.getRawParameterValue (sizeTag);
    t60LowParam      = vts.getRawParameterValue (t60LowTag);
    t60HighParam     = vts.getRawParameterValue (t60HighTag);
    dryWetParam      = vts.getRawParameterValue (dryWetTag);
    preDelayMsParam  = vts.getRawParameterValue (preDelayTag);

    fdnProcs[0] = std::make_unique<FDN> (9);
    fdnProcs[1] = std::make_unique<FDN> (9);
}

BaseFDNProcessor::~BaseFDNProcessor()
{
}

void BaseFDNProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> t60Range (0.1f, 10.0f);
    t60Range.setSkewForCentre (1.0f);

    params.push_back (std::make_unique<AudioParameterFloat> (sizeTag,     "Size",          0.01f, 1.0f, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,   "T60 Low",       t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag,  "T60 High",      t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (dryWetTag,   "Dry/Wet",       0.0f, 1.0f, 1.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (preDelayTag, "PreDelay (Ms)", 0.0f, 500.0f, 20.0f));
}

void BaseFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    AudioProcessor::setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
    {
        delayLine[ch].reset();
        fdnProcs[ch]->reset ((float) sampleRate);
    }

    dryBuffer.setSize (2, samplesPerBlock);
    dryWetProc.reset();
}

void BaseFDNProcessor::releaseResources()
{
}

void BaseFDNProcessor::processBlock (AudioBuffer<float>& buffer)
// {
    ScopedNoDenormals noDenormals;

    dryBuffer.makeCopyOf (buffer, true);

    buffer.applyGain (0.25f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        // pre-delay processing
        const auto delaySamples = (*preDelayMsParam / 1000.0f) * (float) getSampleRate();
        delayLine[ch].setDelay (roundToInt (delaySamples));

        auto* x = buffer.getWritePointer (ch);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            delayLine[ch].write (x[n]);
            x[n] = delayLine[ch].read();
            delayLine[ch].updatePtrs();
        }

        // fdn processing
        fdnProcs[ch]->setSize    (*sizeParam);
        fdnProcs[ch]->setT60Low  (*t60LowParam);
        fdnProcs[ch]->setT60High (*t60HighParam);
        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }

    // dry/wet processing
    dryWetProc.setDryWet (*dryWetParam);
    dryWetProc.processBlock (dryBuffer, buffer);
}

// This creates new instances of the plugin..
#if 0 // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new BaseFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BaseFDNProcessor();
}
#endif
