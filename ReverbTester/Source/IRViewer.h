#ifndef IRVIEWER_H_INCLUDED
#define IRVIEWER_H_INCLUDED

#include "ReverbTesterProcessor.h"

class IRViewer : public Component,
                 private ChangeListener
{
public:
    IRViewer (ReverbTesterProcessor& proc);
    ~IRViewer() {}

    void resized() override;
    void paint (Graphics& g) override;

    void changeListenerCallback (ChangeBroadcaster* source);

private:
    ReverbTesterProcessor& proc;

    TextButton genButton { "Generate IR" };

    AudioFormatManager formatManager;
    std::unique_ptr<AudioThumbnailCache> thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRViewer)
};

#endif // IRVIEWER_H_INCLUDED
