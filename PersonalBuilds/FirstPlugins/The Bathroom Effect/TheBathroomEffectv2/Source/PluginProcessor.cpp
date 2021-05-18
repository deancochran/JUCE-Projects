/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheBathroomEffectv2AudioProcessor::TheBathroomEffectv2AudioProcessor()
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

TheBathroomEffectv2AudioProcessor::~TheBathroomEffectv2AudioProcessor()
{
}

//==============================================================================
const juce::String TheBathroomEffectv2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheBathroomEffectv2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheBathroomEffectv2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheBathroomEffectv2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheBathroomEffectv2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheBathroomEffectv2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TheBathroomEffectv2AudioProcessor::getProgramName (int index)
{
    return {};
}

void TheBathroomEffectv2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TheBathroomEffectv2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
}

void TheBathroomEffectv2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheBathroomEffectv2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TheBathroomEffectv2AudioProcessor::updateFilter()
{
    float freq = *apvts.getRawParameterValue("CUTOFF");
    float res = *apvts.getRawParameterValue("RESONANCE");
    
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq, res);
    
}


void TheBathroomEffectv2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
        
        juce::dsp::AudioBlock <float> block (buffer);
        updateFilter();
        lowPassFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
}

//==============================================================================
bool TheBathroomEffectv2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TheBathroomEffectv2AudioProcessor::createEditor()
{
    return new TheBathroomEffectv2AudioProcessorEditor (*this);
}

//==============================================================================
void TheBathroomEffectv2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TheBathroomEffectv2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheBathroomEffectv2AudioProcessor();
}
//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout TheBathroomEffectv2AudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    juce::NormalisableRange<float> cutoffRange (20.0f, 2000.0f);
    juce::NormalisableRange<float> basicRange (0.1f, 1.0f);
    
    params.add (std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", cutoffRange, 2000.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance", basicRange, 0.5f));
//    std::make_unique<juce::AudioParameterFloat>("ROOMSIZE", "Room Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("DAMPING", "Damping Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("WET", "Wet Level", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("DRY", "Dry Level", basicRange, 0.0f);
    
    
    return params;
}
