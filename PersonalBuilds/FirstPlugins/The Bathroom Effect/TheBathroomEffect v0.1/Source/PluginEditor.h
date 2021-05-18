/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TheBathroomEffectv0_1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TheBathroomEffectv0_1AudioProcessorEditor (TheBathroomEffectv0_1AudioProcessor&);
    ~TheBathroomEffectv0_1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    //==============================================================================
    //FILTER
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    
    juce::Label cutoffLabel     { "Cutoff", "Cutoff" };
    juce::Label resonanceLabel    { "Resonance", "Resonance" };
    
//    //==============================================================================
//    //REVERB
//    juce::Slider drySlider;
//    juce::Slider wetSlider;
//    
//    juce::Label dryLabel     { "Dry", "Dry" };
//    juce::Label wetLabel    { "Wet", "Wet" };
//    
    TheBathroomEffectv0_1AudioProcessor& audioProcessor;
    
public:
    //==============================================================================
    //FILTER
    std::unique_ptr<juce::SliderParameterAttachment> cutoffSliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> resonanceSliderAttachment;
    //==============================================================================
//    //REVERB
//    std::unique_ptr<juce::SliderParameterAttachment> drySliderAttachment;
//    std::unique_ptr<juce::SliderParameterAttachment> wetSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheBathroomEffectv0_1AudioProcessorEditor)
};
