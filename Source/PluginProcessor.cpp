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
	auto delayTimeNoteParam = std::make_unique<AudioParameterInt>(DELAYTIMENOTE_ID, DELAYTIMENOTE_NAME, 0, 7, 3);
	auto feedbackParam = std::make_unique<AudioParameterFloat>(FEEDBACK_ID, FEEDBACK_NAME, 0.0f, 0.98f, 0.6f);
	auto filterAttackParam = std::make_unique<AudioParameterFloat>(FILTERATTACK_ID, FILTERATTACK_NAME, 0.01f, 1.0f, 1.0f);
	auto dryWetParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f, 0.5f);
	auto filterFreqParam = std::make_unique<AudioParameterInt>(FILTERFREQ_ID, FILTERFREQ_NAME, 50, 12000, 500);

	params.push_back(std::move(delayTimeParam));
	params.push_back(std::move(delayTimeNoteParam));
	params.push_back(std::move(feedbackParam));
	params.push_back(std::move(filterAttackParam));
	params.push_back(std::move(dryWetParam));
	params.push_back(std::move(filterFreqParam));
	return { params.begin(), params.end() };
}


//==============================================================================
void AjatarDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	delayTimeSync = false;

	mDelayBufferLength = 15.0 * (samplesPerBlock + sampleRate);

	mDelayTimeSmoothed =  *treeState.getRawParameterValue(DELAYTIME_ID);
	mDelayTimeInSamples = sampleRate * mDelayTimeSmoothed;

	//set delay buffers as float arrays and fill the memories with zeroes
	mDelayBufferLeft = std::make_unique<float[]>(mDelayBufferLength);
	mDelayBufferRight = std::make_unique<float[]>(mDelayBufferLength);

	zeromem(mDelayBufferLeft.get(), mDelayBufferLength * sizeof(float));
	zeromem(mDelayBufferRight.get(), mDelayBufferLength * sizeof(float));

	// Filters
	mFilterHPLeft = std::make_unique<IIRFilter>(); 
	mFilterHPLeft->setCoefficients(IIRCoefficients::makeHighPass(sampleRate, *treeState.getRawParameterValue(FILTERFREQ_ID), 1.0));
	mFilterHPRight = std::make_unique<IIRFilter>(); 
	mFilterHPRight->setCoefficients(IIRCoefficients::makeHighPass(sampleRate, *treeState.getRawParameterValue(FILTERFREQ_ID), 1.0));
}


void AjatarDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	float delayTimeValue = calculateDelayTime();

	float smoothValue = 0.0001;
	float feedbackValue = *treeState.getRawParameterValue(FEEDBACK_ID);
	float dryWetValue = *treeState.getRawParameterValue(DRYWET_ID);
	float filterAttackValue = *treeState.getRawParameterValue(FILTERATTACK_ID);

	float filterFreqValue = *treeState.getRawParameterValue(FILTERFREQ_ID);
	float filterAttack = 48.0f;
	filterAttack = filterAttack + filterAttackValue * (filterFreqValue);
	if (filterAttack > filterFreqValue)
		filterAttack = filterFreqValue;
	mFilterHPLeft->setCoefficients(IIRCoefficients::makeHighPass(getSampleRate(), filterAttack, 0.5));
	mFilterHPRight->setCoefficients(IIRCoefficients::makeHighPass(getSampleRate(), filterAttack, 0.5));

	for (int sample = 0; sample < buffer.getNumSamples(); sample++)
	{
		writeDelay(buffer, sample, delayTimeValue, smoothValue, feedbackValue, dryWetValue);
	}
}


void AjatarDelayAudioProcessor::writeDelay(AudioBuffer<float>& buffer, int sample, float delayTimeValue, float smoothValue, float feedbackValue, float dryWetValue)
{

	//get the float pointers to both channels of buffer
	const float* mainBufferLeftChannel = buffer.getReadPointer(0);
	const float* mainBufferRightChannel = buffer.getReadPointer(1);

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

	// advance the delay write position
	mDelayBufferWritePosition++;
	//Check if the delay buffer position is >= than the lenght of the delay buffer, if so, set it to zero so it wraps to beginning.
	if (mDelayBufferWritePosition >= mDelayBufferLength)
		mDelayBufferWritePosition = 0;
	
	// interpolation to make delaytime changing sound good.
	float interpolatedSampleLeft = cubicInterpolate(mDelayBufferLeft.get(), mDelayReadPosition);
	float interpolatedSampleRight = cubicInterpolate(mDelayBufferRight.get(), mDelayReadPosition);
	float* samplePointerL = &interpolatedSampleLeft;
	float* samplePointerR = &interpolatedSampleRight;

	mFilterHPLeft->processSamples(samplePointerL, 1);
	mFilterHPRight->processSamples(samplePointerR, 1);

	// set the feedbacks to interpolated float * feedbackValue
	mFeedBackLeft = interpolatedSampleLeft * feedbackValue;
	mFeedBackRight = interpolatedSampleRight * feedbackValue;
	//set the buffer samples to current sample * (1 - *drywetValue) + lerp * *dryWetValue
	buffer.setSample(0, sample, buffer.getSample(0, sample) * (1 - dryWetValue) + interpolatedSampleLeft * dryWetValue);
	buffer.setSample(1, sample, buffer.getSample(1, sample) * (1 - dryWetValue) + interpolatedSampleRight * dryWetValue);
}

float AjatarDelayAudioProcessor::calculateDelayTime()
{
	float delayTime;
	if (delayTimeSync)
	{
		mPlayHead = this->getPlayHead();
		mPlayHead->getCurrentPosition(mCurrentPositionInfo);
		mBPM = mCurrentPositionInfo.bpm;

		int noteValue = *treeState.getRawParameterValue(DELAYTIMENOTE_ID);
		delayTime = 60000.0f / mBPM / 250.0f;

		switch (noteValue) {
		case 0:
			break;
		case 1:
			delayTime /= 2;
			break;
		case 2:
			delayTime /= 3;
			break;
		case 3:
			delayTime /= 4;
			break;
		case 4:
			delayTime /= 5;
			break;
		case 5:
			delayTime /= 6;
			break;
		case 6:
			delayTime /= 8;
			break;
		case 7:
			delayTime /= 16;
			break;
		}
	}
	else {
		delayTime = *treeState.getRawParameterValue(DELAYTIME_ID);
	}
	return delayTime;
}

// http://paulbourke.net/miscellaneous/interpolation/
float AjatarDelayAudioProcessor::cubicInterpolate(float buffer[], float readPosition)
{
	int x1 = static_cast<int>(readPosition);
	int x2 = x1 + 1;
	int x0 = x1 - 1;
	int x3 = x2 + 1;

	if (x2 >= mDelayBufferLength)
		x2 -= mDelayBufferLength;
	if (x0 < 0)
		x0 += mDelayBufferLength;
	if (x3 >= mDelayBufferLength)
		x3 -= mDelayBufferLength;

	float y0 = buffer[x0];
	float y1 = buffer[x1];
	float y2 = buffer[x2];
	float y3 = buffer[x3];

	float mu = mDelayReadPosition - x1;

	float a0, a1, a2, a3, mu2;

	mu2 = mu * mu;
	a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
	a1 = y0 - 2.5 * y1 + 2 * y2 - 0.5 * y3;
	a2 = -0.5 * y0 + 0.5 * y2;
	a3 = y1;

	return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
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
