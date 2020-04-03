#include "ReverbTesterEditor.h"

ReverbTesterEditor::ReverbTesterEditor (ReverbTesterProcessor& p) :
    AudioProcessorEditor (p),
    proc (p),
    irViewer (p),
    filePlayer (p)
{
    if (p.getReverbProc())
        procEditor.reset (p.getReverbProc()->createEditor());

    int width = 400;
    int height = 550;
    if (procEditor.get())
    {
        width += procEditor->getWidth();
        height = jmax (height, procEditor->getHeight());
        addAndMakeVisible (procEditor.get());
    }
    setSize (width, height);

    addAndMakeVisible (irViewer);
    addAndMakeVisible (filePlayer);
    addAndMakeVisible (console);

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

void ReverbTesterEditor::resized()
{
    irViewer.setBounds   (0,  10, 400, 150);
    filePlayer.setBounds (0, 160, 400, 150);
    console.setBounds    (0, 320, 400, getHeight() - 330);

    if (procEditor.get())
        procEditor->setTopLeftPosition (400, 0);
}
