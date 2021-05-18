/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheBathroomEffectv3AudioProcessor::TheBathroomEffectv3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
}

TheBathroomEffectv3AudioProcessor::~TheBathroomEffectv3AudioProcessor()
{
}

//==============================================================================
const juce::String TheBathroomEffectv3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheBathroomEffectv3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheBathroomEffectv3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheBathroomEffectv3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheBathroomEffectv3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheBathroomEffectv3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TheBathroomEffectv3AudioProcessor::getProgramName (int index)
{
    return {};
}

void TheBathroomEffectv3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TheBathroomEffectv3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;
        
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
    
    reverbPrepare(spec);
    reset();
    
}

void TheBathroomEffectv3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheBathroomEffectv3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void TheBathroomEffectv3AudioProcessor::updateFilter()
{
    float freq = *apvts.getRawParameterValue("CUTOFF");
    float res = *apvts.getRawParameterValue("RESONANCE");
    
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq, res);
    
}

void TheBathroomEffectv3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock <float> block (buffer);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        updateFilter();
        lowPassFilter.process(juce::dsp::ProcessContextReplacing<float> (block));

        updateReverb();
        reverbProcess(juce::dsp::ProcessContextReplacing<float> (block));
        
}

//==============================================================================
bool TheBathroomEffectv3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TheBathroomEffectv3AudioProcessor::createEditor()
{
    return new TheBathroomEffectv3AudioProcessorEditor (*this);
}

//==============================================================================
void TheBathroomEffectv3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree copyState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary (*xml.get(), destData);
}

void TheBathroomEffectv3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml = getXmlFromBinary (data, sizeInBytes);
    juce::ValueTree copyState = juce::ValueTree::fromXml (*xml.get());
    apvts.replaceState (copyState);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheBathroomEffectv3AudioProcessor();
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout TheBathroomEffectv3AudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    juce::NormalisableRange<float> cutoffRange (20.0f, 2000.0f);
    juce::NormalisableRange<float> basicRange (0.1f, 1.0f);
    
    params.add (std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", cutoffRange, 2000.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance", basicRange, 0.5f));
    params.add (std::make_unique<juce::AudioParameterFloat>("DRY", "Dry Level", basicRange, 0.8f));
    params.add (std::make_unique<juce::AudioParameterFloat>("WET", "Wet Level", basicRange, 0.2f));
    params.add (std::make_unique<juce::AudioParameterFloat>("ROOMSIZE", "Room Size", basicRange, 0.5f));
    params.add (std::make_unique<juce::AudioParameterFloat>("DAMPING", "Damping Size", basicRange, 0.5f));
    params.add (std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width Size", basicRange, 1.0f));
    
    return params;
}

void TheBathroomEffectv3AudioProcessor::updateReverb()
{
    auto inputdB  = juce::Decibels::decibelsToGain  (1.0f);
    auto outputdB = juce::Decibels::decibelsToGain (5.0f);
    if (inputVolume .getGainLinear() != inputdB)     inputVolume.setGainLinear (inputdB);
    if (outputVolume.getGainLinear() != outputdB)   outputVolume.setGainLinear (outputdB);
    
    reverbParameters.dryLevel = *apvts.getRawParameterValue("DRY");
    reverbParameters.wetLevel = *apvts.getRawParameterValue("WET");
    reverbParameters.roomSize = *apvts.getRawParameterValue("ROOMSIZE");
    reverbParameters.damping = *apvts.getRawParameterValue("DAMPING");
    reverbParameters.width = *apvts.getRawParameterValue("WIDTH");
    reverb.setParameters(reverbParameters);
    
}

void TheBathroomEffectv3AudioProcessor::reset()
{
    reverbChain.reset();
}

void TheBathroomEffectv3AudioProcessor::reverbPrepare(const juce::dsp::ProcessSpec &spec)
{
    lastSampleRate = spec.sampleRate;
    
    auto& gainUp = reverbChain.get<0>();
    gainUp.setGainDecibels (24);

    auto& reverbP = reverbChain.get<1>();
    reverbP.setParameters(reverbParameters);
    reverbChain.prepare(spec);
}

void TheBathroomEffectv3AudioProcessor::reverbProcess(const juce::dsp::ProcessContextReplacing<float> &context)
{
    juce::ScopedNoDenormals noDenormals;
    inputVolume.process (context);
    reverb.process(context);
    outputVolume.process (context);
}
