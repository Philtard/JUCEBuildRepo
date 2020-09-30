#pragma once

#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class DelayEditor : public juce::AudioProcessorEditor
{
public:
    explicit DelayEditor(DelayProcessor&, juce::AudioProcessorValueTreeState&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DelayProcessor& processor_;
    juce::AudioProcessorValueTreeState& apvts_;

    juce::Slider delay_gain_slider_ {juce::Slider::SliderStyle::Rotary,
                                     juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider delay_time_slider_ {juce::Slider::SliderStyle::Rotary,
                                     juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Label ui_title_ {"ui_title_label", "Delay"};

    std::unique_ptr<SliderAttachment> delay_gain_slider_attachment_;
    std::unique_ptr<SliderAttachment> delay_time_slider_attachment_;
};
