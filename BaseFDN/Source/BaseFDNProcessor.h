#pragma once

#include "PluginProcessor.h"
#include "FDN.h"
#include "DryWetProcessor.h"

/** Plugin class for basic FDN */
class BaseFDNProcessor : public PluginProcessor<BaseFDNProcessor>
{
public:
    BaseFDNProcessor();
    ~BaseFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;

    AudioProcessorEditor* createEditor() override;
    
private:
    // FDN parameter handles
    std::atomic<float>* sizeParam    = nullptr;
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;

    std::unique_ptr<FDN> fdnProcs[2];

    // Dry/wet processing
    std::atomic<float>* dryWetParam = nullptr;
    AudioBuffer<float> dryBuffer;
    DryWetProcessor dryWetProc;

    // Pre-delay processing
    std::atomic<float>* preDelayMsParam = nullptr;
    DelayLine<float> delayLine[2];

    // Level meter for in Foley's GUI
    foleys::MagicLevelSource* levelSource = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseFDNProcessor)
};
