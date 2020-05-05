#include "FBExperimentProcessor.h"
#include "DelayUtils.h"

namespace
{
    const String lenTag = "length";
    const String fbTag  = "fb";
    const String driveTag = "drive";
    const String smearTag = "smear";
    const String oscGainTag = "oscgain";
    const String oscFreqTag = "oscfreq";
}

FBExperimentProcessor::FBExperimentProcessor()
{
    lenParam = vts.getRawParameterValue (lenTag);
    fbParam  = vts.getRawParameterValue (fbTag);
    driveParam = vts.getRawParameterValue (driveTag);
    smearParam = vts.getRawParameterValue (smearTag);
    oscGainParam = vts.getRawParameterValue (oscGainTag);
    oscFreqParam = vts.getRawParameterValue (oscFreqTag);
}

FBExperimentProcessor::~FBExperimentProcessor()
{
}

void FBExperimentProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> resFreqRange (10.0f, 1000.0f);
    resFreqRange.setSkewForCentre (100.0f);

    NormalisableRange<float> t60Range (0.1f, 3.0f);
    t60Range.setSkewForCentre (0.5f);

    NormalisableRange<float> flutterFreqRange (0.0f, 10.0f);
    flutterFreqRange.setSkewForCentre (1.0f);

    params.push_back (std::make_unique<AudioParameterFloat> (lenTag, "Resonance [Hz]", resFreqRange, 100.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (fbTag,  "Decay Time [s]", t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (driveTag, "Drive [dB]", -30.0f, 30.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (smearTag, "Smear", -0.9f, 0.9f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (oscGainTag, "Flutter Amt", 0.0f, 1.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (oscFreqTag, "Flutter Freq", flutterFreqRange, 0.0f));
}

void FBExperimentProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        procs[ch].reset ((float) sampleRate);
}

void FBExperimentProcessor::releaseResources()
{
}

void FBExperimentProcessor::processBlock (AudioBuffer<float>& buffer)
{
    float delayLenSeconds = 1.0f / *lenParam;
    float fbGain = DelayUtils::calcGainForT60 (delayLenSeconds * (float) getSampleRate(), (float) getSampleRate(), *fbParam);
    float oscGain = pow (*oscGainParam, 3.0f) * 100.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        procs[ch].setDelay (delayLenSeconds * 1000.0f);
        procs[ch].setFB (fbGain);
        procs[ch].setDrive (Decibels::decibelsToGain (driveParam->load()));
        procs[ch].setSmear (*smearParam);

        procs[ch].setOscGain (oscGain);
        procs[ch].setOscFreq (*oscFreqParam);

        procs[ch].process (buffer.getWritePointer (ch), buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new FBExperimentProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FBExperimentProcessor();
}
#endif
