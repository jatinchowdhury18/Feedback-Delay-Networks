#pragma once

#include "PluginProcessor.h"
#include "Schroeder2.h"

class SchroederArch2Processor : public PluginProcessor<SchroederArch2Processor>
{
public:
    SchroederArch2Processor();
    ~SchroederArch2Processor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;

    void updateParams();
    
private:
    std::atomic<float>* fbParam = nullptr;
    std::atomic<float>* delayParam = nullptr;
    std::atomic<float>* gParams[4];
    std::atomic<float>* dParams[4];

    Schroeder2 procs[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SchroederArch2Processor)
};
