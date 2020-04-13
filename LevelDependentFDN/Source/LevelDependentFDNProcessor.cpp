#include "LevelDependentFDNProcessor.h"

namespace
{
    const String attackTag  { "attack" };
    const String releaseTag { "release" };
    const String t60LowTag  { "t60low" };
    const String t60HighTag { "t60high" };
    const String offsetTag  { "offset" };
    const String scaleTag   { "scale" };
}

LevelDependentFDNProcessor::LevelDependentFDNProcessor()
{
    attParam = vts.getRawParameterValue (attackTag);
    relParam = vts.getRawParameterValue (releaseTag);
    t60LowParam  = vts.getRawParameterValue (t60LowTag);
    t60HighParam = vts.getRawParameterValue (t60HighTag);
    offsetParam  = vts.getRawParameterValue (offsetTag);
    scaleParam   = vts.getRawParameterValue (scaleTag);

    const int numDelays = 7;
    fdnProcs[0] = std::make_unique<LevelDetectFDN> (numDelays);
    fdnProcs[1] = std::make_unique<LevelDetectFDN> (numDelays);

    oscilloscope = magicState.addPlotSource ("detectorplot", std::make_unique<foleys::MagicOscilloscope> (0));
}

LevelDependentFDNProcessor::~LevelDependentFDNProcessor()
{
}

void LevelDependentFDNProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> attRange (0.1f, 1000.0f);
    attRange.setSkewForCentre (10.0f);

    NormalisableRange<float> relRange (10.0f, 3000.0f);
    relRange.setSkewForCentre (100.0f);

    NormalisableRange<float> t60Range (0.1f, 10.0f);
    t60Range.setSkewForCentre (1.0f);

    params.push_back (std::make_unique<AudioParameterFloat> (attackTag,  "Attack [Ms]",  attRange, 10.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (releaseTag, "Release [Ms]", relRange, 100.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,  "T60 Low",      t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag, "T60 High",     t60Range, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (offsetTag,  "Offset", 0.0f, 0.5f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> (scaleTag,   "Scale",  0.0f, 2.0f, 1.0f));
}

void LevelDependentFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
    {
        detector[ch].reset ((float) sampleRate);
        detector[ch].setType (DetectorType::Peak);

        fdnProcs[ch]->reset ((float) sampleRate);
        fdnProcs[ch]->prepare (sampleRate, samplesPerBlock);
    }

    detectBuffer.setSize (getTotalNumInputChannels(), samplesPerBlock);
}

void LevelDependentFDNProcessor::releaseResources()
{
    for (int ch = 0; ch < 2; ++ch)
        fdnProcs[ch]->releaseResources();
}

void LevelDependentFDNProcessor::processBlock (AudioBuffer<float>& buffer)
{
    // Level detect processing
    detectBuffer.makeCopyOf (buffer, true);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        detector[ch].setAttackMs  (*attParam);
        detector[ch].setReleaseMs (*relParam);
        detector[ch].processBlock (detectBuffer.getWritePointer (ch), buffer.getNumSamples());
        fdnProcs[ch]->copyDetectBuffer (detectBuffer.getReadPointer (ch), buffer.getNumSamples());
    }

    // FDN processing
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        // fdn processing
        fdnProcs[ch]->setSize    (1.0f);
        fdnProcs[ch]->setT60Low  (*t60LowParam);
        fdnProcs[ch]->setT60High (*t60HighParam);
        fdnProcs[ch]->setScale   (*scaleParam);
        fdnProcs[ch]->setOffset  (*offsetParam);

        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }

    // Send levelDetection signal to oscilloscope
    detectBuffer.applyGain (4.0f);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        FloatVectorOperations::add (detectBuffer.getWritePointer (ch), -1.0f, buffer.getNumSamples());

    oscilloscope->pushSamples (buffer);
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new LevelDependentFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LevelDependentFDNProcessor();
}
#endif
