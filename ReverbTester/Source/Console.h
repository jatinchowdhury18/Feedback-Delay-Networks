#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "JuceHeader.h"

class Console : public Component,
                private Logger
{
public:
    Console();
    ~Console();

    void resized() override;
    void paint (Graphics& g) override;

    void logMessage (const String& text) override;

private:
    CodeDocument codeDoc;
    CodeEditorComponent console { codeDoc, nullptr };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Console)
};

#endif // CONSOLE_H_INCLUDED
