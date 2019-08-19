/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

bool AjatarDelayAudioProcessor::delayTimeSync;

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
	delayTimeSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	delayTimeSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	delayTimeSlider.setRange(0.01f, 2.0f);
	delayTimeSlider.setValue(0.5f);
	delayTimeSlider.setNumDecimalPlacesToDisplay(1);
	delayTimeSlider.textFromValueFunction = [](float value)
	{
		int time = (value * (1000.0f));	
		return juce::String(time) + "ms";
	};
	addAndMakeVisible(&delayTimeSlider);
	addAndMakeVisible(delayTimeLabel);
	delayTimeLabel.setJustificationType(Justification(36));
	delayTimeLabel.setText("Delay Time", dontSendNotification);
	delayTimeLabel.attachToComponent(&delayTimeSlider, false); // [4]

	// tempo synced time
	delayTimeNoteSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAYTIMENOTE_ID, delayTimeNoteSlider);
	delayTimeNoteSlider.setBounds(0, 30, 100, 100);
	delayTimeNoteSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	delayTimeNoteSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	delayTimeNoteSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	delayTimeNoteSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	delayTimeNoteSlider.setRange(0, 7);
	delayTimeNoteSlider.setValue(3);
	addAndMakeVisible(&delayTimeNoteSlider);
	delayTimeNoteSlider.setVisible(false);
//	addAndMakeVisible(delayTimeNoteLabel);
//	delayTimeNoteLabel.setJustificationType(Justification(36));
//	delayTimeNoteLabel.setText("Delay Time", dontSendNotification);
//	delayTimeLabel.attachToComponent(&delayTimeSlider, false); // [4]

	feedbackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FEEDBACK_ID, feedbackSlider);
	feedbackSlider.setBounds(100, 70, 100, 100);
	feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	feedbackSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	feedbackSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	feedbackSlider.setRange(0.0f, 0.98f);
	feedbackSlider.setValue(0.3f);
	feedbackSlider.textFromValueFunction = [](float value)
	{
		int feedback = (value * (100.0f));
		return juce::String(feedback) + "%";
	}; 
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
	smoothSlider.textFromValueFunction = [](float value)
	{
		int smooth = (value * (1000.0f));
		smooth = 11 - smooth;
		return juce::String(smooth);
	}; 
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
	dryWetSlider.textFromValueFunction = [](float value)
	{
		int dryWet = (value * (100.0f));
		return juce::String(dryWet) + "%";
	};
	addAndMakeVisible(&dryWetSlider);
	addAndMakeVisible(dryWetLabel);
	dryWetLabel.setJustificationType(Justification(36));
	dryWetLabel.setText("Dry/Wet", dontSendNotification);
	dryWetLabel.attachToComponent(&dryWetSlider, false); // [4]

	getLookAndFeel().setColour(Label::textColourId, Colour(0xFF2d4924));

	addAndMakeVisible(tempoButton);
	tempoButton.onClick = [this] { changeDelayTimeMode(); };
	tempoButton.setBounds(10, 130, 100, 30);
	tempoButton.setButtonText("time mode");

}

void AjatarDelayAudioProcessorEditor::changeDelayTimeMode()
{
	bool& delayTimeSync = AjatarDelayAudioProcessor::delayTimeSync;
	delayTimeSync = !delayTimeSync;

	if (delayTimeSync)
	{
		delayTimeSlider.setVisible(false);
		delayTimeNoteSlider.setVisible(true);
	}
	else {
		delayTimeSlider.setVisible(true);
		delayTimeNoteSlider.setVisible(false);
	}

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
