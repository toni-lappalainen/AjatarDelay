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

class DelayLookAndFeel : public LookAndFeel_V4
{
public:
	DelayLookAndFeel()
	{
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto radius = jmin(width / 2, height / 2) - 4.0f;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// fill  #f1ece9
		g.setColour(Colour (0xff7ea172));
		g.fillEllipse(rx, ry, rw, rw);

		// outline #b8ccc6
		g.setColour(Colour (0xff2d4924));
		g.drawEllipse(rx, ry, rw, rw, 1.0f);

		Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		// pointer
		g.setColour(Colours::black);
		g.fillPath(p);


	}

	Label* createSliderTextBox(Slider& slider) override
	{
		Label* l = LookAndFeel_V4::createSliderTextBox(slider);
		l->setBorderSize(BorderSize<int>(2, 2, 40, 2));
		l->setColour(Label::outlineColourId, Colours::transparentWhite);
		l->setColour(Label::outlineWhenEditingColourId, Colours::transparentWhite);
		slider.setColour(Slider::textBoxTextColourId, Colour (0xFF2d4924));
		slider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
		
		return l;
	}

	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
		bool isMouseOverButton, bool isButtonDown) override
	{
		auto buttonArea = button.getLocalBounds();
		auto edge = 4;

		buttonArea.removeFromLeft(edge);
		buttonArea.removeFromTop(edge);

		// shadow
		g.setColour(Colours::darkgrey.withAlpha(0.5f));
		g.fillRect(buttonArea);

		auto offset = isButtonDown ? -edge / 2 : -edge;
		buttonArea.translate(offset, offset);

		g.setColour(backgroundColour);
		g.fillRect(buttonArea);
	}
};

class Slider_reverse : public Slider
{
public:
	Slider_reverse() {};
	~Slider_reverse() {};
	double proportionOfLengthToValue(double proportion) override { return Slider::proportionOfLengthToValue(1.0f - proportion); };
	double valueToProportionOfLength(double value) override { return 1.0f - (Slider::valueToProportionOfLength(value)); };
};

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

	void changeDelayTimeMode();

    AjatarDelayAudioProcessor& processor;

	DelayLookAndFeel delayLookAndFeel;

	TextButton tempoButton;

	Slider delayTimeSlider;
	Slider delayTimeNoteSlider;
	Slider feedbackSlider;
	Slider_reverse smoothSlider;
	Slider dryWetSlider;
	Slider filterFreqSlider;

	Label delayTimeLabel;
	Label feedbackLabel;
	Label smoothLabel;
	Label dryWetLabel;
	Label filterFreqLabel;

	Label ajatarLabel;

public:
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> delayTimeSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> delayTimeNoteSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> feedbackSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> smoothSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> dryWetSliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> filterFreqSliderValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AjatarDelayAudioProcessorEditor)
};
