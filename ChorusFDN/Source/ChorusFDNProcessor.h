#pragma once

#include "PluginProcessor.h"
#include "ChorusFDN.h"

class ChorusFDNProcessor : public PluginProcessor<ChorusFDNProcessor>
{
public:
    ChorusFDNProcessor();
    ~ChorusFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;
    std::atomic<float>* oscFreqParam = nullptr;
    std::atomic<float>* oscAmpParam  = nullptr;
    std::atomic<float>* phaseParam   = nullptr;

    std::unique_ptr<ChorusFDN> fdnProcs[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusFDNProcessor)
};
