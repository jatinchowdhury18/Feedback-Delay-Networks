#ifndef FILE_PLAYER_H_INCLUDED
#define FILE_PLAYER_H_INCLUDED

#include "ReverbTesterProcessor.h"

/** Class to playback audio file */
class FilePlayer : public Component,
                   private ChangeListener
{
public:
    FilePlayer (ReverbTesterProcessor& proc);
    ~FilePlayer() {}

    void resized() override;
    void paint (Graphics& g) override;

    void loadFile();
    void playButtonPressed();

    void changeListenerCallback (ChangeBroadcaster* source) override;

private:
    ReverbTesterProcessor& proc;

    TextButton fileButton { "Load File" };
    TextButton playButton { "Play" };

    // Audio waveform
    AudioFormatManager formatManager;
    std::unique_ptr<AudioThumbnailCache> thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilePlayer)
};

#endif // FILE_PLAYER_H_INCLUDED
