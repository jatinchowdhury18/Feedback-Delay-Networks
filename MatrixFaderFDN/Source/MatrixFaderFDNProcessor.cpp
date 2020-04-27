#include "MatrixFaderFDNProcessor.h"

namespace
{
    const String t60LowTag  = "t60Low";
    const String t60HighTag = "t60High";
    const String faderTag   = "fader";
}

MatrixFaderFDNProcessor::MatrixFaderFDNProcessor()
{
    t60LowParam  = vts.getRawParameterValue (t60LowTag);
    t60HighParam = vts.getRawParameterValue (t60HighTag);
    faderParam   = vts.getRawParameterValue (faderTag);

    const int numDelays = 12;
    fdnProcs[0] = std::make_unique<MatrixFaderFDN> (numDelays);
    fdnProcs[1] = std::make_unique<MatrixFaderFDN> (numDelays);

    MixingMatrixUtils::logMatrix (fdnProcs[0]->getMatrix());
}

MatrixFaderFDNProcessor::~MatrixFaderFDNProcessor()
{
}

void MatrixFaderFDNProcessor::addParameters (Parameters& params)
{
    NormalisableRange<float> t60Range (0.1f, 10.0f);
    t60Range.setSkewForCentre (1.0f);

    params.push_back (std::make_unique<AudioParameterFloat> (t60LowTag,  "T60 Low",  t60Range,  0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (t60HighTag, "T60 High", t60Range,  0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> (faderTag,   "Fader", -1.0f, 1.0f, 0.0f));
}

void MatrixFaderFDNProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    PluginProcessor::prepareToPlay (sampleRate, samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        fdnProcs[ch]->reset (sampleRate);
}

void MatrixFaderFDNProcessor::releaseResources()
{
}

void MatrixFaderFDNProcessor::processBlock (AudioBuffer<float>& buffer)
{
    ScopedNoDenormals noDenormals;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        fdnProcs[ch]->setT60Low  (*t60LowParam);
        fdnProcs[ch]->setT60High (*t60HighParam);
        fdnProcs[ch]->setFade    (*faderParam);

        fdnProcs[ch]->processBlock (x, buffer.getNumSamples());
    }
}

// This creates new instances of the plugin..
#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new MatrixFaderFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MatrixFaderFDNProcessor();
}
#endif
