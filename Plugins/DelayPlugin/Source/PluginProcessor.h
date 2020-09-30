#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

class DelayProcessor : public juce::AudioProcessor
{
public:
    DelayProcessor();

    void prepareToPlay(double sampleRate, int blockSize) override;
    void releaseResources() override {}

    bool isBusesLayoutSupported(const BusesLayout&) const override { return true; }

    BusesProperties getBusesProperties()
    {
        auto stereo = juce::AudioChannelSet::stereo();
        return BusesProperties().withOutput("Output", stereo, true);
    }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return juce::String(); }
    void changeProgramName(int, const juce::String& /*newName*/) override {}

    void getStateInformation(juce::MemoryBlock& /*destData*/) override {}
    void setStateInformation(const void* /*data*/, int /*sizeInBytes*/) override {}

private:
    void fillDelayBuffer(int channel,
                         int bufferLength,
                         int delayBufferLength,
                         const float* bufferData);
    void getFromDelayBuffer(juce::AudioBuffer<float>& buffer,
                            int channel,
                            int bufferLength,
                            int delayBufferLength,
                            const float* delayBufferData) const;
    void feedbackDelay(int channel,
                       int bufferLength,
                       int delayBufferLength,
                       float* dryBuffer);

    juce::AudioProcessorValueTreeState apvts_;
    std::atomic<float>* delay_gain_;
    std::atomic<float>* delay_time_;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::AudioBuffer<float> mDelayBuffer_;
    int mWritePosition_ {0};
    int mSampleRate_ {48000};
};
