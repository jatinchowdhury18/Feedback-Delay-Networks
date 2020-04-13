#ifndef REVERBTESTEREDITOR_H_INCLUDED
#define REVERBTESTEREDITOR_H_INCLUDED

#include "ReverbTesterProcessor.h"
#include "Console.h"
#include "IRViewer.h"
#include "FilePlayer.h"

/** GUI class for ReverbTester */
class ReverbTesterEditor : public AudioProcessorEditor
{
public:
    ReverbTesterEditor (ReverbTesterProcessor& p);
    ~ReverbTesterEditor();

    void paint (Graphics& g) override;
    void resized() override;

    void childBoundsChanged (Component* child) override;

private:
    ReverbTesterProcessor& proc;
    std::unique_ptr<AudioProcessorEditor> procEditor;

    IRViewer irViewer;
    FilePlayer filePlayer;
    Console console;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbTesterEditor)
};

#endif // REVERBTESTEREDITOR_H_INCLUDED
