# pragma once

#include "JuceHeader.h"

/**
 * Base class for plugin processors.
 * 
 * Children must override prepareToPlay
 * and releaseResources (from AudioProcessor),
 * as well as processBlock, and addParameters.
 * */
template<class Processor>
class PluginProcessor : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

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
    foleys::MagicProcessorState magicState { *this, vts };

private:
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

// check is template class has addParameters
template<typename T>
class HasAddParameters
{
    typedef char one;
    typedef long two;

    template <typename C> static one test (decltype(&C::addParameters));
    template <typename C> static two test (...);

public:
    enum { value = sizeof (test<T> (0)) == sizeof (char) };
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

    static_assert (HasAddParameters<Processor>::value,
        "Processor class MUST contain a static addParameters function!");
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
    return new foleys::MagicPluginEditor (magicState);
}

template<class Processor>
void PluginProcessor<Processor>::getStateInformation (MemoryBlock& data)
{
    magicState.getStateInformation (data);
}

template<class Processor>
void PluginProcessor<Processor>::setStateInformation (const void* data, int sizeInBytes)
{
    magicState.setStateInformation (data, sizeInBytes, getActiveEditor());
}
