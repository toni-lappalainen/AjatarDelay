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
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
//	mDelayBufferLeft = nullptr;
	//mDelayBufferRight = nullptr;
}

AjatarDelayAudioProcessor::~AjatarDelayAudioProcessor()
{
	//ensure that buffers are nullptr when starting
	/*
	if (mDelayBufferLeft != nullptr)
	{
		delete[] mDelayBufferLeft;
		mDelayBufferLeft = nullptr;
	}
	if (mDelayBufferRight != nullptr)
	{
		delete[] mDelayBufferRight;
		mDelayBufferRight = nullptr;
	}
	*/
}


//==============================================================================
void AjatarDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	mDelayBufferLength = 2.0 * (samplesPerBlock + sampleRate);
	
	mDelayTimeSmoothed = 0.3f;
	mDelayTimeInSamples = sampleRate * mDelayTimeSmoothed;
	//set delay buffers as float arrays and fill the memories with zeroes
		/*
	if (mDelayBufferLeft == nullptr)
	{
		mDelayBufferLeft = new float[mDelayBufferLength];
	}
	zeromem(mDelayBufferLeft, mDelayBufferLength * sizeof(float));

	if (mDelayBufferRight == nullptr)
	{
		mDelayBufferRight = new float[mDelayBufferLength];
	}
	zeromem(mDelayBufferRight, mDelayBufferLength * sizeof(float));
	*/

	mDelayBufferLeft.reset(new float[mDelayBufferLength]);
	mDelayBufferRight.reset(new float[mDelayBufferLength]);
}

void AjatarDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AjatarDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
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

void AjatarDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	//get the float pointers to both channels of buffer
	float* mainBufferLeftChannel = buffer.getWritePointer(0);
	float* mainBufferRightChannel = buffer.getWritePointer(1);

	float smoothValue = 0.006f;
	float delayTimeValue = 0.5f;
	float feedbackValue = 0.9f;

    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
		// calculate the smoothed delaytimes. 
		mDelayTimeSmoothed = mDelayTimeSmoothed - smoothValue * (mDelayTimeSmoothed - delayTimeValue);
		
		// get the delay time in samples
		mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;

		// set the delaybuffers at delayWritePosition to mainBuffers at [sample] location
		mDelayBufferLeft[mDelayBufferWritePosition] = mainBufferLeftChannel[sample];
		mDelayBufferRight[mDelayBufferWritePosition] = mainBufferRightChannel[sample];

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

		float lerpedSampleLeft = mDelayBufferLeft[readPosition_x0];//lerp(mDelayBufferLeft[readPosition_x0], mDelayBufferLeft[readPosition_x1], readPositionFloat);
		float lerpedSampleRight = mDelayBufferRight[readPosition_x0];//lerp(mDelayBufferRight[readPosition_x0], mDelayBufferRight[readPosition_x1], readPositionFloat);

		// set the feedbacks to interpolated float * feedbackValue
		mFeedBackLeft = lerpedSampleLeft * feedbackValue;
		mFeedBackRight = lerpedSampleRight * feedbackValue;
		// advance the delay write position
		mDelayBufferWritePosition++;
		//set the buffer samples to current sample * (1 - *drywetValue) + lerp * *dryWetValue
		buffer.setSample(0, sample, buffer.getSample(0, sample) * (1 - 0.5f) + lerpedSampleLeft * 0.5f);
		buffer.setSample(1, sample, buffer.getSample(1, sample) * (1 - 0.5f) + lerpedSampleRight * 0.5f);
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
    return new AjatarDelayAudioProcessorEditor (*this);
}

//==============================================================================
void AjatarDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AjatarDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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

void AjatarDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String AjatarDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void AjatarDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

