#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 250);
    setResizable(true,true);

    //using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    gainAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "gain", gainSlider);
    thresholdAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "threshold", thresholdSlider);
    ratioAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "ratio", ratioSlider);
    attackAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "attack", attackSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(processorRef.apvts, "release", releaseSlider);

    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    //gainSlider.setRange(-48.0, 0.0);
    //gainSlider.setValue(-1.0);
    //gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);

    // processorRef.expander->setReleaseTime(100.0, processorRef.getSampleRate());
    // processorRef.expander->setAttackTime(100.0, processorRef.getSampleRate());

    thresholdSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
   // thresholdSlider.setRange(-96.0, 0.0);
   // thresholdSlider.setValue(0.0);
   // thresholdSlider.addListener(this);
    addAndMakeVisible(thresholdSlider);

    ratioSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
   // ratioSlider.setRange(0.0, 4.0);
 //   ratioSlider.setValue(1.0);
  //  ratioSlider.addListener(this);
    addAndMakeVisible(ratioSlider);

    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
  //  attackSlider.setRange(0.1, 500.0);
  //  attackSlider.setValue(100.0);
  //  attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);

    releaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
 //   releaseSlider.setRange(0.1, 500.0);
  //  releaseSlider.setValue(100.0);
 //   releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);


    gainLabel.setText ("Gain", juce::dontSendNotification);
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
    //g.fillAll (juce::Colours::blueviolet);

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
/*
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    */

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 5 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = padding;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 3);

    gainSlider.setBounds(sliderStartX, sliderStartY,sliderWidth, sliderHeight);
    thresholdSlider.setBounds(gainSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    ratioSlider.setBounds(thresholdSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    attackSlider.setBounds(ratioSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);
    releaseSlider.setBounds(attackSlider.getRight() + padding, sliderStartY,sliderWidth, sliderHeight);

    //gainSlider.setBoundsRelative(0.5f,0.5f,0.3f,0.3f);
    /*
    thresholdSlider.setBoundsRelative(0.1f,0.5f,0.3f,0.3f);
    ratioSlider.setBoundsRelative(0.2f,0.5f,0.3f,0.3f);
    releaseSlider.setBoundsRelative(0.3f,0.5f,0.3f,0.3f);
    attackSlider.setBoundsRelative(0.4f,0.5f,0.3f,0.3f);
*/
    //gainLabel.setBoundsRelative(0.2f,0.2f,0.3f,0.3f);
    //gainLabel.attachToComponent (&gainSlider, false);
}
/*
void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    if (slider == &gainSlider) {
        //processorRef.gainValue = pow(10, gainSlider.getValue() / 20);
        processorRef.gainValue = *processorRef.apvts.getRawParameterValue("Gain");
    }

    if (slider == &thresholdSlider) {
        processorRef.expander->setThreshold(thresholdSlider.getValue());
    }
    if (slider == &ratioSlider) {
        processorRef.expander->setRatio(ratioSlider.getValue());
    }
    if (slider == &attackSlider) {
        processorRef.expander->setAttackTime(attackSlider.getValue(), processorRef.getSampleRate());
    }
    if (slider == &releaseSlider) {
        processorRef.expander->setReleaseTime(releaseSlider.getValue(), processorRef.getSampleRate());
    }
}
 */
