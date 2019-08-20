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

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(500, 170);

	delayTimeSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAYTIME_ID, delayTimeSlider);
	delayTimeSlider.setBounds(0, 30, 90, 90);
	delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	delayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	delayTimeSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	delayTimeSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	delayTimeSlider.textFromValueFunction = [](float value)
	{
		int time = (value * (1000.0f));	
		return juce::String(time) + "ms";
	};
	delayTimeSlider.setRange(0.01f, 2.0f);
//	delayTimeSlider.setValue(0.5f);
	delayTimeSlider.setNumDecimalPlacesToDisplay(1);
	if (!started)
	{
		addAndMakeVisible(&delayTimeSlider);
		addAndMakeVisible(delayTimeLabel);
	}
	if (!AjatarDelayAudioProcessor::delayTimeSync)
	{
		delayTimeSlider.setVisible(true);
	}
	else
	{
		delayTimeSlider.setVisible(false);
	}
	delayTimeLabel.setJustificationType(Justification(36));
	delayTimeLabel.setText("Delay Time", dontSendNotification);
	delayTimeLabel.attachToComponent(&delayTimeSlider, false); 

	// tempo synced time
	delayTimeNoteSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAYTIMENOTE_ID, delayTimeNoteSlider);
	delayTimeNoteSlider.setBounds(0, 30, 90, 90);
	delayTimeNoteSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	delayTimeNoteSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	delayTimeNoteSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	delayTimeNoteSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	delayTimeNoteSlider.textFromValueFunction = [](int value)
	{
		juce::String note;
		switch (value) {
		case 0:
			note = "1/1";
			break;
		case 1:
			note = "1/2";
			break;
		case 2:
			note = "1/3";
			break;
		case 3:
			note = "1/4";
			break;
		case 4:
			note = "1/5";
			break;
		case 5:
			note = "1/6";
			break;
		case 6:
			note = "1/8";
			break;
		case 7:
			note = "1/16";
			break;
		}
		return note;
	};
	delayTimeNoteSlider.setRange(0, 7, 1);
//	delayTimeNoteSlider.setValue(3);
	addAndMakeVisible(&delayTimeNoteSlider);
	delayTimeNoteSlider.setVisible(false);
	if (AjatarDelayAudioProcessor::delayTimeSync)
	{
		delayTimeNoteSlider.setVisible(true);
		delayTimeLabel.setJustificationType(Justification(36));
		delayTimeLabel.setText("Tempo Sync", dontSendNotification);
		delayTimeLabel.attachToComponent(&delayTimeNoteSlider, false);
	}

	feedbackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FEEDBACK_ID, feedbackSlider);
	feedbackSlider.setBounds(100, 70, 90, 90);
	feedbackSlider.textFromValueFunction = [](float value)
	{
		int feedback = (value * (100.0f));
		return juce::String(feedback) + "%";
	}; 
	feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	feedbackSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	feedbackSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	feedbackSlider.setRange(0.0f, 0.98f);
//	feedbackSlider.setValue(0.3f);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(feedbackLabel);
	feedbackLabel.setJustificationType(Justification(36));
	feedbackLabel.setText("Feedback", dontSendNotification);
	feedbackLabel.attachToComponent(&feedbackSlider, false); 

	filterAttackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FILTERATTACK_ID, filterAttackSlider);
	filterAttackSlider.setBounds(300, 30, 90, 90);
	filterAttackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	filterAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	filterAttackSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	filterAttackSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	filterAttackSlider.textFromValueFunction = [](float value)
	{
		int filterAttack = (value * (100.0f));
		return juce::String(filterAttack) + "%";
	}; 
	filterAttackSlider.setRange(0.01f, 1.0f, 0.01f);
//	filterAttackSlider.setValue(1.0f);
	filterFreqSlider.setSkewFactorFromMidPoint(0.7f);
	addAndMakeVisible(&filterAttackSlider);
	addAndMakeVisible(filterAttackLabel);
	filterAttackLabel.setText("Filter Attack", dontSendNotification);
	filterAttackLabel.setJustificationType(Justification(36));
	filterAttackLabel.attachToComponent(&filterAttackSlider, false); 

	filterFreqSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, FILTERFREQ_ID, filterFreqSlider);
	filterFreqSlider.setBounds(200, 70, 90, 90);
	filterFreqSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	filterFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	filterFreqSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	filterFreqSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	filterFreqSlider.setTextValueSuffix("Hz");
	filterFreqSlider.setRange(50, 12000, 5);
//	filterFreqSlider.setValue(500);
	filterFreqSlider.setSkewFactorFromMidPoint(1200);
	addAndMakeVisible(&filterFreqSlider);
	addAndMakeVisible(filterFreqLabel);
	filterFreqLabel.setJustificationType(Justification(36));
	filterFreqLabel.setText("HP Filter Freq", dontSendNotification);
	filterFreqLabel.attachToComponent(&filterFreqSlider, false);

	dryWetSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DRYWET_ID, dryWetSlider);
	dryWetSlider.setBounds(400, 30, 90, 90);
	dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	dryWetSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);	
	dryWetSlider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
	dryWetSlider.textFromValueFunction = [](float value)
	{
		int dryWet = (value * (100.0f));
		return juce::String(dryWet) + "%";
	};
	dryWetSlider.setRange(0.0f, 1.0f);
//	dryWetSlider.setValue(0.5f);
	addAndMakeVisible(&dryWetSlider);
	addAndMakeVisible(dryWetLabel);
	dryWetLabel.setJustificationType(Justification(36));
	dryWetLabel.setText("Dry/Wet", dontSendNotification);
	dryWetLabel.attachToComponent(&dryWetSlider, false);

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
		delayTimeLabel.setJustificationType(Justification(36));
		delayTimeLabel.setText("Tempo Sync", dontSendNotification);
		delayTimeLabel.attachToComponent(&delayTimeNoteSlider, false);

	}
	else {
		delayTimeSlider.setVisible(true);
		delayTimeNoteSlider.setVisible(false);
		delayTimeLabel.setJustificationType(Justification(36));
		delayTimeLabel.setText("Delay Time", dontSendNotification);
		delayTimeLabel.attachToComponent(&delayTimeSlider, false); 
	}

}

AjatarDelayAudioProcessorEditor::~AjatarDelayAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void AjatarDelayAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(Colour (0xffc7cb85));

	g.setColour(Colour (0xFF2d4924));
	g.setFont(22.0f);
	g.drawFittedText("Ajatar Delay", 200, 140,100, 22, Justification::horizontallyCentred, 1);
	
}

void AjatarDelayAudioProcessorEditor::resized()
{
}
