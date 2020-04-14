#include "AmpFeedbackFDNProcessor.h"

namespace
{
    const String sizeTag      = "size";
    const String t60LowTag    = "t60Low";
    const String t60HighTag   = "t60High";
    const String fbDelayTag   = "fbDelay";
    const String fbDriveTag   = "fbDrive";
    const String fbGainTag    = "fbGain";
    const String cutoffTag    = "cutoff";
    const String fbOscAmpTag  = "fboscamp";
    const String fbOscFreqTag = "fboscfreq";
}

AmpFeedbackFDNProcessor::AmpFeedbackFDNProcessor()
{
    sizeParam      = vts.getRawParameterValue (sizeTag);
    t60LowParam    = vts.getRawParameterValue (t60LowTag);
    t60HighParam   = vts.getRawParameterValue (t60HighTag);
    fbDelayMsParam = vts.getRawParameterValue (fbDelayTag);
    fbDriveDBParam = vts.getRawParameterValue (fbDriveTag);
    fbGainDBParam  = vts.getRawParameterValue (fbGainTag);
    cutoffHzParam  = vts.getRawParameterValue (cutoffTag);
    fbOscAmpParam  = vts.getRawParameterValue (fbOscAmpTag);
    fbOscFreqParam = vts.getRawParameterValue (fbOscFreqTag);

    const int numDelays = 5;
    fdnProcs[0] = std::make_unique<AmpFeedbackFDN> (numDelays);
    fdnProcs[1] = std::make_unique<AmpFeedbackFDN> (numDelays);
}

AmpFeedbackFDNProcessor::~AmpFeedbackFDNProcessor()
{
}

void AmpFeedbackFDNProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> t60Range (0.1f, 10.0f);
    t60Range.setSkewForCentre (1.0f);

    NormalisableRange<float> freqRange (20.0f, 20000.0f);
    freqRange.setSkewForCentre (2000.0f);

    NormalisableRange<float> vibRange (0.1f, 10.0f);
    vibRange.setSkewForCentre (0.2f);

    params.push_back (std::make_unique<AudioParameterFloat> (sizeTag,      "Size",     0.01f, 1.0f, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,    "T60 Low",  t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag,   "T60 High", t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbDelayTag,   "FB Delay", 1.0f, 1000.0f, 250.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbDriveTag,   "FB Drive", 0.0f, 12.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbGainTag,    "FB Gain",  -60.0f, 0.0f, -25.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (cutoffTag,    "Cutoff",   freqRange, 2000.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbOscAmpTag,  "FB Osc",   0.0f, 100.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbOscFreqTag, "FB Osc Freq", vibRange, 0.2f));
}

void AmpFeedbackFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        fdnProcs[ch]->reset ((float) sampleRate);
}

void AmpFeedbackFDNProcessor::releaseResources()
{
}

void AmpFeedbackFDNProcessor::processBlock (AudioBuffer<float>& buffer)
{
    ScopedNoDenormals noDenormals;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        // fdn processing
        fdnProcs[ch]->setSize    (*sizeParam);
        fdnProcs[ch]->setT60Low  (*t60LowParam);
        fdnProcs[ch]->setT60High (*t60HighParam);

        fdnProcs[ch]->setFBTime  (*fbDelayMsParam);
        fdnProcs[ch]->setDrive   (Decibels::decibelsToGain (fbDriveDBParam->load()));
        fdnProcs[ch]->setFBGain  (Decibels::decibelsToGain (fbGainDBParam->load()));
        fdnProcs[ch]->setCutoff  (*cutoffHzParam);
        fdnProcs[ch]->setSineParams (*fbOscAmpParam, *fbOscFreqParam);

        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new AmpFeedbackFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmpFeedbackFDNProcessor();
}
#endif
