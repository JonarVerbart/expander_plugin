#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor (&p), processorRef (p) {
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 250);
    setResizable(true,true);

    // Parameter attachments
    gainAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "gain", gainSlider);
    thresholdAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "threshold", thresholdSlider);
    ratioAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "ratio", ratioSlider);
    attackAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "attack", attackSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "release", releaseSlider);

    // Parameter sliders
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    addAndMakeVisible(gainSlider);

    thresholdSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    addAndMakeVisible(thresholdSlider);

    ratioSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    addAndMakeVisible(ratioSlider);

    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    addAndMakeVisible(attackSlider);

    releaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    addAndMakeVisible(releaseSlider);

    // Parameter labels
    gainLabel.setText ("Makeup Gain", juce::dontSendNotification);
    gainLabel.attachToComponent (&gainSlider, false);
    addAndMakeVisible (gainLabel);

    thresholdLabel.setText ("Threshold", juce::dontSendNotification);
    thresholdLabel.attachToComponent (&thresholdSlider, false);
    addAndMakeVisible (thresholdLabel);

    ratioLabel.setText ("Ratio", juce::dontSendNotification);
    ratioLabel.attachToComponent (&ratioSlider, false);
    addAndMakeVisible (ratioLabel);

    attackLabel.setText ("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent (&attackSlider, false);
    addAndMakeVisible (attackLabel);

    releaseLabel.setText ("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent (&releaseSlider, false);
    addAndMakeVisible (releaseLabel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor

    // Some values for placing UI elements relatively
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 5 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = padding;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 3);
    // Draw sliders relative to window size
    thresholdSlider.setBounds(sliderStartX + padding, sliderStartY,sliderWidth, sliderHeight);
    ratioSlider.setBounds(thresholdSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    attackSlider.setBounds(ratioSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    releaseSlider.setBounds(attackSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    gainSlider.setBounds(releaseSlider.getRight(), sliderStartY,sliderWidth, sliderHeight);
}