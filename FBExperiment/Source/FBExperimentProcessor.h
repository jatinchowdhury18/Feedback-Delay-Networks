#pragma once

#include "PluginProcessor.h"
#include "FBProc.h"

class FBExperimentProcessor : public PluginProcessor<FBExperimentProcessor>
{
public:
    FBExperimentProcessor();
    ~FBExperimentProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    std::atomic<float>* lenParam = nullptr;
    std::atomic<float>* fbParam  = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* smearParam = nullptr;
    std::atomic<float>* oscGainParam = nullptr;
    std::atomic<float>* oscFreqParam = nullptr;

    FBProc procs[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FBExperimentProcessor)
};
