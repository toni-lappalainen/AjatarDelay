/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define M_PI 3.14159265f

#define DELAYTIME_ID "delaytime"
#define DELAYTIME_NAME "DelayTime"
#define DELAYTIMENOTE_ID "delaytimenote"
#define DELAYTIMENOTE_NAME "DelayTimeNote"

#define FEEDBACK_ID "feedback"
#define FEEDBACK_NAME "Feedback"
#define DRYWET_ID "drywet"
#define DRYWET_NAME "DryWet"
#define FILTERATTACK_ID "smooth"
#define FILTERATTACK_NAME "Smooth"
#define FILTERFREQ_ID "filterfreq"
#define  FILTERFREQ_NAME "FIlterFreq"

//==============================================================================
/**
*/
class AjatarDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AjatarDelayAudioProcessor();
    ~AjatarDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	float cubicInterpolate(float buffer[], float readPosition);

	float calculateDelayTime();

	void writeDelay(AudioBuffer<float>& buffer, int sample, float delayTimeValue, float smoothValue, float feedbackValue, float dryWetValue);

	AudioProcessorValueTreeState treeState;
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:

	std::unique_ptr<float[]> mDelayBufferLeft;
	std::unique_ptr<float[]> mDelayBufferRight;

	float mDelayReadPosition{ 0.0f };
	float mDelayTimeInSamples{ 0.0f };
	float mDelayTimeSmoothed{ 0.0f };

	float mFeedBackLeft{ 0.0f };
	float mFeedBackRight{ 0.0f };

	int mDelayBufferWritePosition{ 0 };
	int mDelayBufferLength{ 0 };

	AudioPlayHead* mPlayHead;
	AudioPlayHead::CurrentPositionInfo mCurrentPositionInfo;

	float mBPM{120.0f};

	// filters
	std::unique_ptr<IIRFilter> mFilterHPLeft;
	std::unique_ptr<IIRFilter> mFilterHPRight;

	int mFilterHPFreq{ 500 };


public:

	static bool delayTimeSync;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AjatarDelayAudioProcessor)
};
