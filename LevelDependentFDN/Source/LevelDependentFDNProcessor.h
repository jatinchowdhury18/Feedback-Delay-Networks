#pragma once

#include "PluginProcessor.h"
#include "LevelDetector.h"
#include "LevelDetectFDN.h"

class LevelDependentFDNProcessor : public PluginProcessor<LevelDependentFDNProcessor>
{
public:
    LevelDependentFDNProcessor();
    ~LevelDependentFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    LevelDetector detector[2];
    std::atomic<float>* attParam = nullptr;
    std::atomic<float>* relParam = nullptr;
    AudioBuffer<float> detectBuffer;

    std::unique_ptr<LevelDetectFDN> fdnProcs[2];
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;
    std::atomic<float>* offsetParam  = nullptr;
    std::atomic<float>* scaleParam   = nullptr;

    // Oscilloscope for level detector
    foleys::MagicPlotSource*    oscilloscope = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelDependentFDNProcessor)
};
