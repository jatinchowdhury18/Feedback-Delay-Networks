#include "Console.h"

Console::Console()
{
    addAndMakeVisible (console);
    console.setReadOnly (true);
    console.setLineNumbersShown (false);
    console.setScrollbarThickness (8);

    Logger::setCurrentLogger (this);
}

Console::~Console()
{
    Logger::setCurrentLogger (nullptr);
}

void Console::resized()
{
    console.setBounds (getLocalBounds().reduced (8));
}

void Console::paint (Graphics& g)
{
    g.setColour (Colours::white);
    g.drawRect (getLocalBounds().reduced (5));
}

void Console::logMessage (const String& text)
{
    codeDoc.insertText (console.getCaretPos(), text + "\n");
    console.scrollToKeepCaretOnScreen();
}
