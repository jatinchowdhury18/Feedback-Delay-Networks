#pragma once

#include "PluginProcessor.h"
#include "FDN.h"
#include "DryWetProcessor.h"

class BaseFDNProcessor : public PluginProcessor<BaseFDNProcessor>
{
public:
    BaseFDNProcessor();
    ~BaseFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    std::atomic<float>* sizeParam    = nullptr;
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;

    std::unique_ptr<FDN> fdnProcs[2];

    std::atomic<float>* dryWetParam = nullptr;
    AudioBuffer<float> dryBuffer;
    DryWetProcessor dryWetProc;

    std::atomic<float>* preDelayMsParam = nullptr;
    DelayLine delayLine[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseFDNProcessor)
};
