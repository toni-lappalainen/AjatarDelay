/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AjatarDelayAudioProcessorEditor::AjatarDelayAudioProcessorEditor (AjatarDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(400, 130);

	delayTimeSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAYTIME_ID, delayTimeSlider);
	delayTimeSlider.setBounds(0, 0, 100, 100);
	delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	delayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	delayTimeSlider.setRange(0.01f, 2.0f);
	delayTimeSlider.setValue(0.5f);
	addAndMakeVisible(&delayTimeSlider);

	feedbackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FEEDBACK_ID, feedbackSlider);
	feedbackSlider.setBounds(100, 0, 100, 100);
	feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	feedbackSlider.setRange(0.0f, 0.98f);
	feedbackSlider.setValue(0.3f);
	addAndMakeVisible(&feedbackSlider);


	smoothSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, SMOOTH_ID, smoothSlider);
	smoothSlider.setBounds(300, 0, 100, 100);
	smoothSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	smoothSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	smoothSlider.setRange(0.001f, 0.02f);
	smoothSlider.setValue(0.006f);
	addAndMakeVisible(&smoothSlider);

	dryWetSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DRYWET_ID, dryWetSlider);
	dryWetSlider.setBounds(200, 0, 100, 100);
	dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	dryWetSlider.setRange(0.0f, 1.0f);
	dryWetSlider.setValue(0.5f);
	addAndMakeVisible(&dryWetSlider);
}

AjatarDelayAudioProcessorEditor::~AjatarDelayAudioProcessorEditor()
{
}

//==============================================================================
void AjatarDelayAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Ajatar Delay", getLocalBounds(), Justification::centredBottom, 1);
}

void AjatarDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
