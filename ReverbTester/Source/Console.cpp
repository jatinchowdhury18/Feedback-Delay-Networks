#include "Console.h"

Console::Console()
{
    addAndMakeVisible (console);
    console.setReadOnly (true);
    console.setLineNumbersShown (false);
    console.setScrollbarThickness (8);

    Logger::setCurrentLogger (this); // redirect global logging to here
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

    // write to console
    for (auto&& m : logMessages)
    {
        codeDoc.insertText (console.getCaretPos(), m + "\n");
        console.scrollToKeepCaretOnScreen();
    }
}

void Console::logMessage (const String& text)
{
    {
        // Add messages to pending list
        const ScopedLock sl (logMessagesLock);
        pendingLogMessages.add (text);

        // print to console asynchronously so as to not block other threads
        triggerAsyncUpdate();
    }

    DBG (text); // write to standard debug output as well
}
