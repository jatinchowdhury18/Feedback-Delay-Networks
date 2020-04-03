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

void Console::handleAsyncUpdate()
{
    StringArray logMessages;

    {
        const ScopedLock sl (logMessagesLock);
        pendingLogMessages.swapWith (logMessages);
    }

    for (auto&& m : logMessages)
    {
        codeDoc.insertText (console.getCaretPos(), m + "\n");
        console.scrollToKeepCaretOnScreen();
    }
}

void Console::logMessage (const String& text)
{
    {
        const ScopedLock sl (logMessagesLock);
        pendingLogMessages.add (text);
        triggerAsyncUpdate();
    }

    DBG (text);
}
