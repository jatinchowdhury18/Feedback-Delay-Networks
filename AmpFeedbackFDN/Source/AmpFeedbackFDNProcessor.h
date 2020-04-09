#pragma once

#include "PluginProcessor.h"
#include "AmpFeedbackFDN.h"

class AmpFeedbackFDNProcessor : public PluginProcessor<AmpFeedbackFDNProcessor>
{
public:
    AmpFeedbackFDNProcessor();
    ~AmpFeedbackFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    std::atomic<float>* sizeParam    = nullptr;
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;
    std::atomic<float>* fbDelayMsParam = nullptr;
    std::atomic<float>* fbDriveDBParam = nullptr;
    std::atomic<float>* fbGainDBParam  = nullptr;
    std::atomic<float>* cutoffHzParam  = nullptr;

    std::unique_ptr<AmpFeedbackFDN> fdnProcs[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpFeedbackFDNProcessor)
};
