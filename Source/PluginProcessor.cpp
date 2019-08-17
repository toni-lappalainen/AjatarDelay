/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AjatarDelayAudioProcessor::AjatarDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	treeState(*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

AjatarDelayAudioProcessor::~AjatarDelayAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout AjatarDelayAudioProcessor::createParameterLayout()
{
	std::vector <std::unique_ptr<RangedAudioParameter>> params;
	auto delayTimeParam = std::make_unique<AudioParameterFloat>(DELAYTIME_ID, DELAYTIME_NAME, 0.01f, 2.0f, 0.5f);
	auto feedbackParam = std::make_unique<AudioParameterFloat>(FEEDBACK_ID, FEEDBACK_NAME, 0.0f, 0.98f, 0.6f);
	auto smoothParam = std::make_unique<AudioParameterFloat>(SMOOTH_ID, SMOOTH_NAME, 0.001f, 0.02f, 0.006f);
	auto dryWetParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f, 0.5f);

	params.push_back(std::move(delayTimeParam));
	params.push_back(std::move(feedbackParam));
	params.push_back(std::move(smoothParam));
	params.push_back(std::move(dryWetParam));
	return { params.begin(), params.end() };
}


//==============================================================================
void AjatarDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mDelayBufferLength = 2.0 * (samplesPerBlock + sampleRate);

	mDelayTimeSmoothed = *treeState.getRawParameterValue(DELAYTIME_ID);
	mDelayTimeInSamples = sampleRate * mDelayTimeSmoothed;

	//set delay buffers as float arrays and fill the memories with zeroes
	mDelayBufferLeft = std::make_unique<float[]>(mDelayBufferLength);
	mDelayBufferRight = std::make_unique<float[]>(mDelayBufferLength);

	zeromem(mDelayBufferLeft.get(), mDelayBufferLength * sizeof(float));
	zeromem(mDelayBufferRight.get(), mDelayBufferLength * sizeof(float));

}

void AjatarDelayAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AjatarDelayAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void AjatarDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	//get the float pointers to both channels of buffer
	const float* mainBufferLeftChannel = buffer.getReadPointer(0);
	const float* mainBufferRightChannel = buffer.getReadPointer(1);


	for (int sample = 0; sample < buffer.getNumSamples(); sample++)
	{
		float delayTimeValue = *treeState.getRawParameterValue(DELAYTIME_ID);
		float feedbackValue = *treeState.getRawParameterValue(FEEDBACK_ID);
		float smoothValue = *treeState.getRawParameterValue(SMOOTH_ID);
		float dryWetValue = *treeState.getRawParameterValue(DRYWET_ID);

		// calculate the smoothed delaytimes. 
		mDelayTimeSmoothed = mDelayTimeSmoothed - smoothValue * (mDelayTimeSmoothed - delayTimeValue);

		// get the delay time in samples
		mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;

		// set the delaybuffers at delayWritePosition to mainBuffers at [sample] location
		mDelayBufferLeft[mDelayBufferWritePosition] = mainBufferLeftChannel[sample] + mFeedBackLeft;
		mDelayBufferRight[mDelayBufferWritePosition] = mainBufferRightChannel[sample] + mFeedBackRight;

		// set delay read position to delay write position - delay time in samples
		// and check if delay read position is smaller than zero
		// if it is, add delaybuffer length to it.
		mDelayReadPosition = mDelayBufferWritePosition - mDelayTimeInSamples;
		if (mDelayReadPosition < 0)
			mDelayReadPosition += mDelayBufferLength;

		// do the linear interpolation calculations here
		// ints for readposition x and x1. readpositionInt_x is delay read position. x1 is x + 1
		// subtract x from delay read position to get read position float.
		// check if x1 is >= delaybuffer length, if so, decrease the length from it
		// then create new floats by doing the lerp(dBufferL[x],dBufferR[x1], rposfloat)

		int readPosition_x0 = static_cast<int>(mDelayReadPosition);
		int readPosition_x1 = readPosition_x0 + 1;
		float readPositionFloat = mDelayReadPosition - readPosition_x0;

		if (readPosition_x1 >= mDelayBufferLength)
			readPosition_x1 -= mDelayBufferLength;

		float lerpedSampleLeft = lerp(mDelayBufferLeft[readPosition_x0], mDelayBufferLeft[readPosition_x1], readPositionFloat);
		float lerpedSampleRight = lerp(mDelayBufferRight[readPosition_x0], mDelayBufferRight[readPosition_x1], readPositionFloat);

		// set the feedbacks to interpolated float * feedbackValue
		mFeedBackLeft = lerpedSampleLeft * feedbackValue;
		mFeedBackRight = lerpedSampleRight * feedbackValue;
		// advance the delay write position
		mDelayBufferWritePosition++;
		//set the buffer samples to current sample * (1 - *drywetValue) + lerp * *dryWetValue
		buffer.setSample(0, sample, buffer.getSample(0, sample) * (1 - dryWetValue) + lerpedSampleLeft * dryWetValue);
		buffer.setSample(1, sample, buffer.getSample(1, sample) * (1 - dryWetValue) + lerpedSampleRight * dryWetValue);
		//Check if the delay buffer position is >= than the lenght of the delay buffer, if so, set it to zero so it wraps to beginning.
		if (mDelayBufferWritePosition >= mDelayBufferLength)
			mDelayBufferWritePosition = 0;
	}
}

float AjatarDelayAudioProcessor::lerp(float x0, float x1, float phase) {
	return (1 - phase) * x0 + phase * x1;
}

//==============================================================================
bool AjatarDelayAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AjatarDelayAudioProcessor::createEditor()
{
	return new AjatarDelayAudioProcessorEditor(*this);
}

//==============================================================================
void AjatarDelayAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void AjatarDelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new AjatarDelayAudioProcessor();
}

//==============================================================================
const String AjatarDelayAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool AjatarDelayAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool AjatarDelayAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool AjatarDelayAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double AjatarDelayAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int AjatarDelayAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int AjatarDelayAudioProcessor::getCurrentProgram()
{
	return 0;
}

void AjatarDelayAudioProcessor::setCurrentProgram(int index)
{
}

const String AjatarDelayAudioProcessor::getProgramName(int index)
{
	return {};
}

void AjatarDelayAudioProcessor::changeProgramName(int index, const String& newName)
{
}

