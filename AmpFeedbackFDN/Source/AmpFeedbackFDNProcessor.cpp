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

    params.push_back (std::make_unique<AudioParameterFloat> (sizeTag,    "Size",     0.01f, 1.0f, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,  "T60 Low",  t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag, "T60 High", t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbDelayTag, "FB Delay", 1.0f, 1000.0f, 250.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbDriveTag, "FB Drive", -30.0f, 30.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbGainTag,  "FB Gain",  -60.0f, -15.0f, -25.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (cutoffTag,  "Cutoff",  500.0f, 20000.0f, 2000.0f));
}

void AmpFeedbackFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    AudioProcessor::setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);

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

        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#if 1 // Set this flag to run with ReverbTester
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
