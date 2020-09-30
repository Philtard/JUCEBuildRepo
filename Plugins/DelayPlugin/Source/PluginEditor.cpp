#include "PluginProcessor.h"
#include "PluginEditor.h"

DelayEditor::DelayEditor(DelayProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p)
    , processor_(p)
    , apvts_(vts)
{
    delay_gain_slider_attachment_ = std::make_unique<SliderAttachment>(
        apvts_, "delay_gain", delay_gain_slider_);
    addAndMakeVisible(delay_gain_slider_);

    delay_time_slider_attachment_ = std::make_unique<SliderAttachment>(
        apvts_, "delay_time", delay_time_slider_);
    addAndMakeVisible(delay_time_slider_);

    addAndMakeVisible(ui_title_);
    setSize(350, 100);
}

void DelayEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void DelayEditor::resized()
{
    auto bounds = getLocalBounds();

    ui_title_.setBounds(bounds.removeFromLeft(150));
    ui_title_.setJustificationType(juce::Justification::centred);

    delay_time_slider_.setBounds(bounds.removeFromLeft(100));
    delay_gain_slider_.setBounds(bounds.removeFromLeft(100));
}
