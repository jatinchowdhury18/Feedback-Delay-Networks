# pragma once

#include "JuceHeader.h"

/**
    Base class for plugin processors.
    
    Children must override prepareToPlay
    and releaseResources (from AudioProcessor),
    as well as processBlock, and addParameters.
*/
class PluginProcessor : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& name) override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    virtual void processBlock (AudioBuffer<float>&) = 0;

    bool hasEditor() const override;
    AudioProcessorEditor* createEditor() override;

    void getStateInformation (MemoryBlock& data) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

protected:
    using Parameters = std::vector<std::unique_ptr<RangedAudioParameter>>;
    virtual void addParameters (Parameters& params) = 0;
    AudioProcessorValueTreeState vts;

private:
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
