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
class TheBathroomEffectv2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TheBathroomEffectv2AudioProcessorEditor (TheBathroomEffectv2AudioProcessor&);
    ~TheBathroomEffectv2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
        //FILTER
        juce::Slider cutoffSlider;
        juce::Slider resonanceSlider;
        
        juce::Label cutoffLabel     { "Cutoff", "Cutoff" };
        juce::Label resonanceLabel    { "Resonance", "Resonance" };
public:
    //==============================================================================
        //FILTER
        std::unique_ptr<juce::SliderParameterAttachment> cutoffSliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> resonanceSliderAttachment;
    TheBathroomEffectv2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheBathroomEffectv2AudioProcessorEditor)
};
