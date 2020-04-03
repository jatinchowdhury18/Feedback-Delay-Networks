#include "PluginProcessor.h"

PluginProcessor::PluginProcessor() :
    AudioProcessor (BusesProperties()
        .withInput ("Input", AudioChannelSet::stereo(), true)
        .withOutput ("Output", AudioChannelSet::stereo(), true)),
    vts (*this, nullptr, Identifier ("Parameters"), createParameterLayout())
{
}

PluginProcessor::~PluginProcessor() {}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    Parameters params;
    // addParameters (params);
    return { params.begin(), params.end() };
}

const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const { return false; }
bool PluginProcessor::producesMidi() const { return false; } 
bool PluginProcessor::isMidiEffect() const { return false; }

double PluginProcessor::getTailLengthSeconds() const
{
    return 1.0; // default 1 second since these are reverb effects
}
    
int PluginProcessor::getNumPrograms() { return 1; }
int PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram (int) {}
const String PluginProcessor::getProgramName (int) { return {}; }
void PluginProcessor::changeProgramName (int, const String&) {}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PluginProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    processBlock (buffer);
}

bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

void PluginProcessor::getStateInformation (MemoryBlock& data)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, data);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vts.state.getType()))
            vts.replaceState (ValueTree::fromXml (*xmlState));
}
