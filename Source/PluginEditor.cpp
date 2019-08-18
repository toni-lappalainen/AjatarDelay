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
	setLookAndFeel(&delayLookAndFeel);
	//setLookAndFeel(nullptr);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(500, 170);

	delayLookAndFeel.setColour(Slider::textBoxTextColourId, Colours::red);

	delayTimeSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAYTIME_ID, delayTimeSlider);
	delayTimeSlider.setBounds(0, 30, 100, 100);
	delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	delayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	delayTimeSlider.setTextValueSuffix("s");
	delayTimeSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	delayTimeSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	delayTimeSlider.setRange(0.01f, 2.0f);
	delayTimeSlider.setValue(0.5f);
	addAndMakeVisible(&delayTimeSlider);
	addAndMakeVisible(delayTimeLabel);
	delayTimeLabel.setJustificationType(Justification(36));
	delayTimeLabel.setText("Delay Time", dontSendNotification);
	delayTimeLabel.attachToComponent(&delayTimeSlider, false); // [4]

	feedbackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FEEDBACK_ID, feedbackSlider);
	feedbackSlider.setBounds(100, 70, 100, 100);
	feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	feedbackSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	feedbackSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	feedbackSlider.setRange(0.0f, 0.98f);
	feedbackSlider.setValue(0.3f);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(feedbackLabel);
	feedbackLabel.setJustificationType(Justification(36));
	feedbackLabel.setText("Feedback", dontSendNotification);
	feedbackLabel.attachToComponent(&feedbackSlider, false); // [4]

	smoothSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, SMOOTH_ID, smoothSlider);
	smoothSlider.setBounds(200, 30, 100, 100);
	smoothSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	smoothSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	smoothSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	smoothSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	smoothSlider.textFromValueFunction = nullptr;
	smoothSlider.setRange(0.001f, 0.01f, 0.001f);
	smoothSlider.setValue(0.006f);
	smoothSlider.setNumDecimalPlacesToDisplay(3);
	addAndMakeVisible(&smoothSlider);
	addAndMakeVisible(smoothLabel);
	smoothLabel.setText("Smooth", dontSendNotification);
	smoothLabel.setJustificationType(Justification(36));
	smoothLabel.attachToComponent(&smoothSlider, false); // [4]

	filterFreqSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FILTERFREQ_ID, filterFreqSlider);
	filterFreqSlider.setBounds(300, 70, 100, 100);
	filterFreqSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	filterFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	filterFreqSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	filterFreqSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	filterFreqSlider.setTextValueSuffix("Hz");
	filterFreqSlider.setRange(50, 12000, 5);
	filterFreqSlider.setValue(500);
	filterFreqSlider.setSkewFactorFromMidPoint(1200);
	addAndMakeVisible(&filterFreqSlider);
	addAndMakeVisible(filterFreqLabel);
	filterFreqLabel.setJustificationType(Justification(36));
	filterFreqLabel.setText("Filter Freq", dontSendNotification);
	filterFreqLabel.attachToComponent(&filterFreqSlider, false); // [4]

	dryWetSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DRYWET_ID, dryWetSlider);
	dryWetSlider.setBounds(400, 30, 100, 100);
	dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	dryWetSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	dryWetSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	dryWetSlider.setRange(0.0f, 1.0f);
	dryWetSlider.setValue(0.5f);
	addAndMakeVisible(&dryWetSlider);
	addAndMakeVisible(dryWetLabel);
	dryWetLabel.setJustificationType(Justification(36));
	dryWetLabel.setText("Dry/Wet", dontSendNotification);
	dryWetLabel.attachToComponent(&dryWetSlider, false); // [4]

	getLookAndFeel().setColour(Label::textColourId, Colour(0xFF2d4924));


}

AjatarDelayAudioProcessorEditor::~AjatarDelayAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void AjatarDelayAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)#f9e3be
	g.fillAll(Colour (0xffc7cb85));

	g.setColour(Colour (0xFF2d4924));
	g.setFont(22.0f);
	g.drawFittedText("Ajatar Delay", 200, 140,100, 22, Justification::horizontallyCentred, 1);
	
}

void AjatarDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
