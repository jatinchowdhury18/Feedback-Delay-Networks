#pragma once

#include "PluginProcessor.h"
#include "MatrixFaderFDN.h"

class MatrixFaderFDNProcessor : public PluginProcessor<MatrixFaderFDNProcessor>
{
public:
    MatrixFaderFDNProcessor();
    ~MatrixFaderFDNProcessor();
    
    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;
    
private:
    std::atomic<float>* t60LowParam  = nullptr;
    std::atomic<float>* t60HighParam = nullptr;
    std::atomic<float>* faderParam   = nullptr;

    std::unique_ptr<MatrixFaderFDN> fdnProcs[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixFaderFDNProcessor)
};
