#include "IRViewer.h"

enum
{
    cacheSize = 5,
    thumbSamples = 2,
    hash = 0x2345,
};

IRViewer::IRViewer (ReverbTesterProcessor& proc) :
    proc (proc)
{
    addAndMakeVisible (genButton);
    genButton.onClick = [=, &proc] { proc.setState (ReverbTesterProcessor::IR); };

    formatManager.registerBasicFormats();
    thumbnailCache = std::make_unique<AudioThumbnailCache> (cacheSize);
    thumbnail = std::make_unique<AudioThumbnail> (thumbSamples, formatManager, *thumbnailCache.get());

    thumbnail->addChangeListener (this);
    proc.addChangeListener (this);
}

void IRViewer::resized()
{
    genButton.setBounds (5, (getHeight() - 15) / 2, 90, 30);
}

void IRViewer::paint (Graphics& g)
{
    g.setColour (Colours::red);
    Rectangle<int> thumbRect (105, 5, getWidth() - 110, getHeight() - 10);
    thumbnail->drawChannels (g, thumbRect, 0, thumbnail->getTotalLength(), 1.0f);
}

void IRViewer::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == thumbnail.get())
    {
        repaint();
    }
    else if (source == &proc)
    {
        thumbnail->reset (proc.getMainBusNumOutputChannels(), proc.getSampleRate());
        auto& buffer = proc.getIR();
        thumbnail->addBlock (0, buffer, 0, buffer.getNumSamples());
    }
}
