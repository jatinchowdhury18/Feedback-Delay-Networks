#include "ChorusFDNProcessor.h"

namespace
{
    const String t60LowTag  = "t60Low";
    const String t60HighTag = "t60High";
    const String oscFreqTag = "oscFreq";
    const String oscAmpTag  = "oscAmp";
    const String phaseTag   = "phase";
}

ChorusFDNProcessor::ChorusFDNProcessor()
{
    t60LowParam  = vts.getRawParameterValue (t60LowTag);
    t60HighParam = vts.getRawParameterValue (t60HighTag);
    oscFreqParam = vts.getRawParameterValue (oscFreqTag);
    oscAmpParam  = vts.getRawParameterValue (oscAmpTag);
    phaseParam   = vts.getRawParameterValue (phaseTag);

    const int numDelays = 7;
    fdnProcs[0] = std::make_unique<ChorusFDN> (numDelays);
    fdnProcs[1] = std::make_unique<ChorusFDN> (numDelays);

    MixingMatrixUtils::logMatrix (fdnProcs[0]->getMatrix());
}

ChorusFDNProcessor::~ChorusFDNProcessor()
{
}

void ChorusFDNProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> t60Range (0.1f, 10.0f);
    t60Range.setSkewForCentre (1.0f);

    NormalisableRange<float> freqRange (0.0f, 30.0f);
    freqRange.setSkewForCentre (5.0f);

    NormalisableRange<float> ampRange (0.0f, 50.0f);
    ampRange.setSkewForCentre (10.0f);

    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,  "T60 Low",  t60Range,  0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag, "T60 High", t60Range,  0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (oscFreqTag, "Osc Freq", freqRange, 1.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (oscAmpTag,  "Osc Amp",  ampRange,  7.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (phaseTag,   "Phase Scramble",  0.0f, 1.0f, 0.25f));
}

void ChorusFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        fdnProcs[ch]->prepare (sampleRate, samplesPerBlock);
}

void ChorusFDNProcessor::releaseResources()
{
}

void ChorusFDNProcessor::processBlock (AudioBuffer<float>& buffer)
{
    ScopedNoDenormals noDenormals;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        fdnProcs[ch]->setT60Low  (*t60LowParam);
        fdnProcs[ch]->setT60High (*t60HighParam);
        fdnProcs[ch]->setOscFreq (*oscFreqParam);
        fdnProcs[ch]->setOscAmp  (*oscAmpParam);
        fdnProcs[ch]->setPhase   (*phaseParam);
        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new ChorusFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusFDNProcessor();
}
#endif
