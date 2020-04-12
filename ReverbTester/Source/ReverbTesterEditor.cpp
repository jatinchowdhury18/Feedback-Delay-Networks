#include "ReverbTesterEditor.h"

enum
{
    width = 400,
    height = 550,
};

ReverbTesterEditor::ReverbTesterEditor (ReverbTesterProcessor& p) :
    AudioProcessorEditor (p),
    proc (p),
    irViewer (p),
    filePlayer (p)
{
    if (p.getReverbProc())
        procEditor.reset (p.getReverbProc()->createEditor());

    if (procEditor.get())
    {
        setSize (width + procEditor->getWidth(), jmax ((int) height, procEditor->getHeight()));
        addAndMakeVisible (procEditor.get());
    }
    else
    {
        setSize (width, height);
    }

    addAndMakeVisible (irViewer);
    addAndMakeVisible (filePlayer);
    addAndMakeVisible (console);

    setResizable(true, true);
    auto screenBounds = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    setResizeLimits (width, height, screenBounds.getWidth(), screenBounds.getHeight());

    Logger::writeToLog ("Starting...");
}

ReverbTesterEditor::~ReverbTesterEditor()
{
}

void ReverbTesterEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::red);
    g.drawLine (400.0f, 0.0f, 400.0f, 0.0f, 2.0f);
}

void ReverbTesterEditor::childBoundsChanged (Component* child)
{
    if (child == procEditor.get())
        setSize (width + procEditor->getWidth(), jmax ((int) height, procEditor->getHeight()));
}

void ReverbTesterEditor::resized()
{
    irViewer.setBounds   (0,  10, 400, 150);
    filePlayer.setBounds (0, 160, 400, 150);
    console.setBounds    (0, 320, 400, getHeight() - 330);

    if (procEditor.get())
    {
        procEditor->setTopLeftPosition (400, 0);
        procEditor->setSize (getWidth() - width, jmin (getHeight(), procEditor->getHeight()));
    }
}
