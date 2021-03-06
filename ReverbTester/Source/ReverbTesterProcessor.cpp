#include "ReverbTesterProcessor.h"
#include "ReverbTesterEditor.h"

ReverbTesterProcessor::ReverbTesterProcessor (AudioProcessor* proc) :
    reverbProcessor (proc)
{
    readAheadThread.startThread (5);
}

ReverbTesterProcessor::~ReverbTesterProcessor()
{
    source.setSource (nullptr);
}

void ReverbTesterProcessor::addParameters (Parameters&)
{
}

void ReverbTesterProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    loadMeasurer.reset (sampleRate, samplesPerBlock);
    source.prepareToPlay (samplesPerBlock, sampleRate);

    if (reverbProcessor.get())
        reverbProcessor->prepareToPlay (sampleRate, samplesPerBlock);
}

void ReverbTesterProcessor::releaseResources()
{
    source.releaseResources();

    if (reverbProcessor.get())
        reverbProcessor->releaseResources();
}

void ReverbTesterProcessor::processBlock (AudioBuffer<float>& buffer)
{
    AudioProcessLoadMeasurer::ScopedTimer loadTimer (loadMeasurer);

    if (state == None)
    {
        buffer.clear();

        if (reverbProcessor.get())
        {
            MidiBuffer midi;
            reverbProcessor->processBlock (buffer, midi);
        }

        return;
    }

    else if (state == IR)
    {
        buffer.clear();
        if (startIR)
        {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                buffer.setSample (ch, buffer.getNumSamples() / 2, 1.0f);
            startIR = false;
        }

        if (reverbProcessor.get())
        {
            MidiBuffer midi;
            reverbProcessor->processBlock (buffer, midi);
        }

        int samplesToWrite = jmin (buffer.getNumSamples(), irBuffer.getNumSamples() - irSampleCount);
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            irBuffer.copyFrom (ch, irSampleCount, buffer, ch, 0, samplesToWrite);

        irSampleCount += samplesToWrite;

        if (irSampleCount + getBlockSize() >= irBuffer.getNumSamples())
            setState (None);
    }

    else if (state == File)
    {
        AudioSourceChannelInfo channelInfo (buffer);
        source.getNextAudioBlock (channelInfo);

        if (reverbProcessor.get())
        {
            MidiBuffer midi;
            reverbProcessor->processBlock (buffer, midi);
        }
    }
}

void ReverbTesterProcessor::setState (State newState)
{
    if (newState == IR)
    {
        startIR = true;
        irBuffer.clear();
        irBuffer.setSize (getMainBusNumOutputChannels(), 2 * (int) getSampleRate());
        irSampleCount = 0;
        Logger::writeToLog ("Generating IR...");
    }
    else if (newState == File)
    {
        source.setPosition (0.0);
    }
    else // newState == None
    {
        if (state == IR)
        {
            irBuffer.setSize (getMainBusNumOutputChannels(), irSampleCount, true);
            sendChangeMessage();
            Logger::writeToLog ("Finished Generating IR!");
        }
    }

    state = newState;
}

void ReverbTesterProcessor::loadFile (AudioFormatReader* reader)
{
    source.stop();
    source.releaseResources();
    source.setSource (nullptr); // delete old readerSource before creating new one
    
    readerSource = std::make_unique<AudioFormatReaderSource> (reader, true);
    source.setSource (readerSource.get(), 4 * 4096, &readAheadThread, reader->sampleRate);
    source.prepareToPlay (getBlockSize(), getSampleRate());
}

AudioProcessorEditor* ReverbTesterProcessor::createEditor()
{
    return new ReverbTesterEditor (*this);
}

#ifndef BUILD_WITH_REVERB_TESTER // don't define this function if the plugin being tested defines it!
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor();
}
#endif
