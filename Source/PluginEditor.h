/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AjatarDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AjatarDelayAudioProcessorEditor (AjatarDelayAudioProcessor&);
    ~AjatarDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AjatarDelayAudioProcessor& processor;

	Slider delayTimeSlider;
	Slider feedbackSlider;
	Slider smoothSlider;
	Slider dryWetSlider;

public:
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> delayTimeSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> feedbackSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> smoothSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> dryWetSliderValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AjatarDelayAudioProcessorEditor)
};
