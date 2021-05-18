/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TheBathroomEffectv0_1AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TheBathroomEffectv0_1AudioProcessor();
    ~TheBathroomEffectv0_1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    
    
    
    //==============================================================================
//    void reverbChainPrepare (const juce::dsp::ProcessSpec& spec);
//    void reverbProcess (const juce::dsp::ProcessContextReplacing<float>& context);
//    void updateReverbParameters();
//    
//
//    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::Reverb> reverbChain;
//    juce::dsp::Gain<float> inputVolume, outputVolume;
    
    //==============================================================================
    void updateFilter();
    
    
    
    void reset() override;

    


private:
    //==============================================================================
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients <float>> lowPassFilter;
    //==============================================================================
//    juce::dsp::Reverb reverb;
//    juce::dsp::Reverb::Parameters reverbParameters;
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    float lastSampleRate;
    //double sampleRate = 0.0;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheBathroomEffectv0_1AudioProcessor)
};
