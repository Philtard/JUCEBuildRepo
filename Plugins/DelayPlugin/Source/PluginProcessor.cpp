#include "PluginProcessor.h"
#include "PluginEditor.h"

DelayProcessor::DelayProcessor()
    : apvts_(*this, nullptr, "PARAMETERS", createParameters())
{
    delay_gain_ = apvts_.getRawParameterValue("delay_gain");
    delay_time_ = apvts_.getRawParameterValue("delay_time");
}

void DelayProcessor::prepareToPlay(double sampleRate, int blockSize)
{
    const auto numInputChannels = getTotalNumInputChannels();
    const auto delayBufferSize = 2 * (sampleRate + blockSize);

    mDelayBuffer_.setSize(numInputChannels, delayBufferSize);
    mSampleRate_ = static_cast<int>(sampleRate);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)

{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    const auto bufferLength = buffer.getNumSamples();
    const auto delayBufferLength = mDelayBuffer_.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = mDelayBuffer_.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);

        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData);
        getFromDelayBuffer(
            buffer, channel, bufferLength, delayBufferLength, delayBufferData);
        feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);
    }

    mWritePosition_ += bufferLength;
    mWritePosition_ %= delayBufferLength;
}
void DelayProcessor::fillDelayBuffer(int channel,
                                     const int bufferLength,
                                     const int delayBufferLength,
                                     const float* bufferData)
{
    const float gain = delay_gain_->load();

    if (delayBufferLength > bufferLength + mWritePosition_)
    {
        mDelayBuffer_.copyFromWithRamp(
            channel, mWritePosition_, bufferData, bufferLength, gain, gain);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - mWritePosition_;
        mDelayBuffer_.copyFromWithRamp(
            channel, mWritePosition_, bufferData, bufferRemaining, gain, gain);
        mDelayBuffer_.copyFromWithRamp(
            channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
    }
}

void DelayProcessor::getFromDelayBuffer(juce::AudioBuffer<float>& buffer,
                                        int channel,
                                        const int bufferLength,
                                        const int delayBufferLength,
                                        const float* delayBufferData) const
{
    int delayTime = delay_time_->load();
    const int readPosition = static_cast<int>(delayBufferLength + mWritePosition_
                                              - (mSampleRate_ * delayTime / 1000))
                             % delayBufferLength;

    if (delayBufferLength > bufferLength + readPosition)
    {
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.copyFrom(
            channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
    }
}

void DelayProcessor::feedbackDelay(int channel,
                                   const int bufferLength,
                                   const int delayBufferLength,
                                   float* dryBuffer)
{
    if (delayBufferLength > bufferLength + mWritePosition_)
    {
        mDelayBuffer_.addFromWithRamp(
            channel, mWritePosition_, dryBuffer, bufferLength, 0.8, 0.8);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - mWritePosition_;

        mDelayBuffer_.addFromWithRamp(
            channel, bufferRemaining, dryBuffer, bufferRemaining, 0.8, 0.8);
        mDelayBuffer_.addFromWithRamp(
            channel, 0, dryBuffer, bufferLength - bufferRemaining, 0.8, 0.8);
    }
}

juce::AudioProcessorEditor* DelayProcessor::createEditor()
{
    return new DelayEditor(*this, apvts_);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "delay_gain", "Delay Gain", 0, 1, 0.8));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "delay_time", "Delay Time ms", 1, 2000, 40));

    return {params.begin(), params.end()};
}
