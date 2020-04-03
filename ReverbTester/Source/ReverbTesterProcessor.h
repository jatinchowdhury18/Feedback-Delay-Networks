#pragma once

#include "PluginProcessor.h"

class ReverbTesterProcessor : public PluginProcessor<ReverbTesterProcessor>,
                              public ChangeBroadcaster
{
public:
    ReverbTesterProcessor (AudioProcessor* proc = nullptr);
    ~ReverbTesterProcessor();
    
    enum State
    {
        None,
        IR,
        File,
    };

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer) override;

    AudioProcessorEditor* createEditor() override;
    
    void setState (State newState);
    State getState() const { return state; }

    const AudioBuffer<float>& getIR() { return irBuffer; }

    void loadFile (AudioFormatReader* reader);
    AudioTransportSource& getTransportSource() { return source; }

    AudioProcessor* getReverbProc() { return reverbProcessor.get(); }

private:
    State state = None;
    std::unique_ptr<AudioProcessor> reverbProcessor;

    bool startIR = false;
    AudioBuffer<float> irBuffer;
    int irSampleCount = 0;

    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource source;
    TimeSliceThread readAheadThread { "readAheadThread" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbTesterProcessor)
};
