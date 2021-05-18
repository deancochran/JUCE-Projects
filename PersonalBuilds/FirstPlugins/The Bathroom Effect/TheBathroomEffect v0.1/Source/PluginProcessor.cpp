/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheBathroomEffectv0_1AudioProcessor::TheBathroomEffectv0_1AudioProcessor()
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
//    apvts.addParameterListener ("CUTOFF", this);
//    apvts.addParameterListener ("RESONANCE", this);
    
//    apvts.addParameterListener ("DRY", this);
//    apvts.addParameterListener ("WET", this);
}

TheBathroomEffectv0_1AudioProcessor::~TheBathroomEffectv0_1AudioProcessor()
{
//    apvts.removeParameterListener ("CUTOFF", this);
//    apvts.removeParameterListener ("RESONANCE", this);
    
//    apvts.removeParameterListener ("DRY", this);
//    apvts.removeParameterListener ("WET", this);
}

//==============================================================================
const juce::String TheBathroomEffectv0_1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheBathroomEffectv0_1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv0_1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheBathroomEffectv0_1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheBathroomEffectv0_1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheBathroomEffectv0_1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheBathroomEffectv0_1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheBathroomEffectv0_1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TheBathroomEffectv0_1AudioProcessor::getProgramName (int index)
{
    return {};
}

void TheBathroomEffectv0_1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TheBathroomEffectv0_1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
    
    //updateReverbParameters();
}

void TheBathroomEffectv0_1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheBathroomEffectv0_1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TheBathroomEffectv0_1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    
//    reverbParameters.roomSize = *apvts.getRawParameterValue("ROOMSIZE");
//    reverbParameters.damping = *apvts.getRawParameterValue("DAMPING");
//    reverbParameters.freezeMode = 0.0f;
//    reverbParameters.width = *apvts.getRawParameterValue("WIDTH");
//    reverbParameters.wetLevel = *apvts.getRawParameterValue("WET");
//    reverbParameters.dryLevel = *apvts.getRawParameterValue("DRY");
//    reverb.setParameters(reverbParameters);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    juce::dsp::AudioBlock<float> sampleBlock (buffer);
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateFilter();
    //updateReverbParameters();
    
    lowPassFilter.process (juce::dsp::ProcessContextReplacing<float> (sampleBlock));
    
    //reverbProcess(juce::dsp::ProcessContextReplacing<float> (sampleBlock));
}

//==============================================================================
bool TheBathroomEffectv0_1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TheBathroomEffectv0_1AudioProcessor::createEditor()
{
    return new TheBathroomEffectv0_1AudioProcessorEditor (*this);
}

//==============================================================================
void TheBathroomEffectv0_1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree copyState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary (*xml.get(), destData);
}

void TheBathroomEffectv0_1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new TheBathroomEffectv0_1AudioProcessor();
}

void TheBathroomEffectv0_1AudioProcessor::reset()
{
    lowPassFilter.reset();
    //reverbChain.reset();
}

void TheBathroomEffectv0_1AudioProcessor::updateFilter()
{
    float freq = *apvts.getRawParameterValue("CUTOFF");
    float res = *apvts.getRawParameterValue("RESONANCE");
        
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq, res);
}

juce::AudioProcessorValueTreeState::ParameterLayout TheBathroomEffectv0_1AudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    juce::NormalisableRange<float> cutoffRange (20.0f, 2000.0f);
    juce::NormalisableRange<float> basicRange (0.0f, 1.0f);
    
    params.add (std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", cutoffRange, 2000.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance", basicRange, 0.0f));
//    std::make_unique<juce::AudioParameterFloat>("ROOMSIZE", "Room Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("DAMPING", "Damping Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width Size", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("WET", "Wet Level", basicRange, 0.0f);
//    std::make_unique<juce::AudioParameterFloat>("DRY", "Dry Level", basicRange, 0.0f);
    
    
    return params;
}


//==============================================================================
//void TheBathroomEffectv0_1AudioProcessor::reverbChainPrepare (const juce::dsp::ProcessSpec& spec)
//{
//    lastSampleRate = spec.sampleRate;
//
//    auto& gainUp = reverbChain.get<0>();
//    gainUp.setGainDecibels (24);
//
//    auto& reverbP = reverbChain.get<1>();
//    reverbP.setParameters(reverbParameters);
//    reverbChain.prepare(spec);
//}
//
//void TheBathroomEffectv0_1AudioProcessor::reverbProcess (const juce::dsp::ProcessContextReplacing<float>& context)
//{
//    juce::ScopedNoDenormals noDenormals;
//    inputVolume.process (context);
//    reverb.process(context);
//    outputVolume.process (context);
//}
//
//void TheBathroomEffectv0_1AudioProcessor::updateReverbParameters()
//{
//    auto inputdB  = juce::Decibels::decibelsToGain  (1.0f);
//    auto outputdB = juce::Decibels::decibelsToGain (5.0f);
//    if (inputVolume .getGainLinear() != inputdB)
//        {inputVolume.setGainLinear (inputdB);}
//    if (outputVolume.getGainLinear() != outputdB)
//        {outputVolume.setGainLinear (outputdB);}
//}
