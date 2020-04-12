#include "FilePlayer.h"

enum
{
    cacheSize = 5,
    thumbSamples = 512,
    hash = 0x2346,
};

FilePlayer::FilePlayer (ReverbTesterProcessor& proc) :
    proc (proc)
{
    addAndMakeVisible (fileButton);
    fileButton.onClick = [=] { loadFile(); };

    addAndMakeVisible (playButton);
    playButton.onClick = [=] { playButtonPressed(); };
    playButton.setEnabled (false);

    formatManager.registerBasicFormats();
    thumbnailCache = std::make_unique<AudioThumbnailCache> (cacheSize);
    thumbnail = std::make_unique<AudioThumbnail> (thumbSamples, formatManager, *thumbnailCache.get());

    thumbnail->addChangeListener (this);
    proc.getTransportSource().addChangeListener (this);
}

void FilePlayer::resized()
{
    fileButton.setBounds (5,     getHeight() / 3 - 15, 90, 30);
    playButton.setBounds (5, 2 * getHeight() / 3 - 15, 90, 30);
}

void FilePlayer::paint (Graphics& g)
{
    g.setColour (Colours::yellow);
    Rectangle<int> thumbRect (105, 5, getWidth() - 110, getHeight() - 10);
    thumbnail->drawChannels (g, thumbRect, 0, thumbnail->getTotalLength(), 1.0f);
}

void FilePlayer::loadFile()
{
    FileChooser fileChooser ("Select file", File(), formatManager.getWildcardForAllFormats(), true);

    if (fileChooser.browseForFileToOpen())
    {
        auto result = fileChooser.getResult();
        Logger::writeToLog ("Loading file: " + result.getFileName());

        thumbnail->setSource (new FileInputSource (result));
        playButton.setEnabled (true);

        auto reader = formatManager.createReaderFor (result);
        proc.loadFile (reader);
        Logger::writeToLog ("File loaded!");
    }
}

void FilePlayer::playButtonPressed()
{
    proc.setState (ReverbTesterProcessor::File);

    if (! proc.getTransportSource().isPlaying())
        proc.getTransportSource().start();
    else
        proc.getTransportSource().stop();
}

void FilePlayer::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == thumbnail.get())
    {
        // thumbnail loading has progressed
        repaint();
    }
    else if (source == &proc.getTransportSource())
    {
        // play state changed...
        if (proc.getTransportSource().isPlaying())
        {
            playButton.setButtonText ("Stop");
            Logger::writeToLog ("File playback started!");
        }
        else
        {
            playButton.setButtonText ("Play");
            Logger::writeToLog ("File playback stopped!");
        }
        
    }
}
