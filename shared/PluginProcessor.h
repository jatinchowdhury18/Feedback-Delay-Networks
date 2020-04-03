# pragma once

#include "JuceHeader.h"

/**
    Base class for plugin processors.
    
    Children must override prepareToPlay
    and releaseResources (from AudioProcessor),
    as well as processBlock, and addParameters.
*/
template<class Processor>
class PluginProcessor : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();

    const String getName() const { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; } 
    bool isMidiEffect() const override { return false; }

    double getTailLengthSeconds() const override;
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const String getProgramName (int) override { return {}; }
    void changeProgramName (int, const String&) override {}

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    virtual void processBlock (AudioBuffer<float>&) = 0;

    bool hasEditor() const override;
    AudioProcessorEditor* createEditor() override;

    void getStateInformation (MemoryBlock& data) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

protected:
    using Parameters = std::vector<std::unique_ptr<RangedAudioParameter>>;
    AudioProcessorValueTreeState vts;

private:
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

template<class Processor>
PluginProcessor<Processor>::PluginProcessor() :
    AudioProcessor (BusesProperties()
        .withInput ("Input", AudioChannelSet::stereo(), true)
        .withOutput ("Output", AudioChannelSet::stereo(), true)),
    vts (*this, nullptr, Identifier ("Parameters"), createParameterLayout())
{
}

template<class Processor>
PluginProcessor<Processor>::~PluginProcessor() {}

template<class Processor>
AudioProcessorValueTreeState::ParameterLayout PluginProcessor<Processor>::createParameterLayout()
{
    Parameters params;
    Processor::addParameters (params);
    return { params.begin(), params.end() };
}

template<class Processor>
double PluginProcessor<Processor>::getTailLengthSeconds() const
{
    return 1.0; // default 1 second since these are reverb effects
}

template<class Processor>
bool PluginProcessor<Processor>::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // only supports mono and stereo (for now)
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // input and output layout must be the same
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    
    return true;
}

template<class Processor>
void PluginProcessor<Processor>::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    processBlock (buffer);
}

template<class Processor>
bool PluginProcessor<Processor>::hasEditor() const
{
    return true;
}

template<class Processor>
AudioProcessorEditor* PluginProcessor<Processor>::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

template<class Processor>
void PluginProcessor<Processor>::getStateInformation (MemoryBlock& data)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, data);
}

template<class Processor>
void PluginProcessor<Processor>::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vts.state.getType()))
            vts.replaceState (ValueTree::fromXml (*xmlState));
}
