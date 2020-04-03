#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "JuceHeader.h"

class Console : public Component,
                private AsyncUpdater,
                private Logger
{
public:
    Console();
    ~Console();

    void resized() override;
    void paint (Graphics& g) override;

private:
    void handleAsyncUpdate() override;
    void logMessage (const String& text) override;

    CriticalSection logMessagesLock;
    StringArray pendingLogMessages;

    CodeDocument codeDoc;
    CodeEditorComponent console { codeDoc, nullptr };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Console)
};

#endif // CONSOLE_H_INCLUDED
