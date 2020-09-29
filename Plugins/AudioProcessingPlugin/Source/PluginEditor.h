#pragma once

#include "PluginProcessor.h"

class Editor : public juce::AudioProcessorEditor
{
public:
    explicit Editor(DelayProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
};
