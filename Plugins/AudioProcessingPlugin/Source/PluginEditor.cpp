#include "PluginProcessor.h"
#include "PluginEditor.h"

Editor::Editor(DelayProcessor& p)
    : AudioProcessorEditor(&p)
{
    setSize(400, 300);
}

void Editor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void Editor::resized()
{
}
