/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicChorusv2AudioProcessor::BasicChorusv2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
}

BasicChorusv2AudioProcessor::~BasicChorusv2AudioProcessor()
{
}

//==============================================================================
const juce::String BasicChorusv2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicChorusv2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicChorusv2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicChorusv2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicChorusv2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicChorusv2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicChorusv2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicChorusv2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicChorusv2AudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicChorusv2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicChorusv2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    chorus.prepare(spec);
    chorus.reset();
}

void BasicChorusv2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicChorusv2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicChorusv2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> sampleBlock (buffer);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateChorus();
    chorus.process (juce::dsp::ProcessContextReplacing<float> (sampleBlock));

}

//==============================================================================
bool BasicChorusv2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicChorusv2AudioProcessor::createEditor()
{
    return new BasicChorusv2AudioProcessorEditor (*this);
}

//==============================================================================
void BasicChorusv2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree copyState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary (*xml.get(), destData);
}

void BasicChorusv2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicChorusv2AudioProcessor();
}


void BasicChorusv2AudioProcessor::reset()
{
    chorus.reset();
}

juce::AudioProcessorValueTreeState::ParameterLayout BasicChorusv2AudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    juce::NormalisableRange<float> basicRange (0.0f, 1.0f, 0.01f);
    juce::NormalisableRange<float> feedbackRange (-1.0f, 1.0f, 0.01f);

    params.add (std::make_unique<juce::AudioParameterInt>   ("RATE", "Rate", 0.0f, 10.0f, 0.0f));
    params.add (std::make_unique<juce::AudioParameterFloat> ("DEPTH", "Depth", basicRange, 0.0f));
    params.add (std::make_unique<juce::AudioParameterFloat> ("DELAY", "Centre Delay", basicRange, 0.0));
    params.add (std::make_unique<juce::AudioParameterFloat> ("FEEDBACK", "Feedback", feedbackRange, 0.0f));
    params.add (std::make_unique<juce::AudioParameterFloat> ("MIX", "Mix",  basicRange, 0.0f));


    
    return params;
}

void BasicChorusv2AudioProcessor::updateChorus()
{
//    std::cout << "RATE" <<*apvts.getRawParameterValue("RATE")<< std::endl;
//    std::cout << "DEPTH" <<*apvts.getRawParameterValue("DEPTH")<< std::endl;
//    std::cout << "DELAY" <<*apvts.getRawParameterValue("DELAY")<< std::endl;
//    std::cout << "FEEDBACK" <<*apvts.getRawParameterValue("FEEDBACK")<< std::endl;
//    std::cout << "MIX" <<*apvts.getRawParameterValue("MIX")<< std::endl;
    chorus.setRate(*apvts.getRawParameterValue("RATE"));
    chorus.setDepth(*apvts.getRawParameterValue("DEPTH"));
    chorus.setCentreDelay(*apvts.getRawParameterValue("DELAY"));
    chorus.setFeedback(*apvts.getRawParameterValue("FEEDBACK"));
    chorus.setMix(*apvts.getRawParameterValue("MIX"));
    
}
