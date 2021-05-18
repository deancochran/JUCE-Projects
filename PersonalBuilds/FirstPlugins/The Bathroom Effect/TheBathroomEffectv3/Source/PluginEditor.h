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
class TheBathroomEffectv3AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TheBathroomEffectv3AudioProcessorEditor (TheBathroomEffectv3AudioProcessor&);
    ~TheBathroomEffectv3AudioProcessorEditor() override;

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
        //==============================================================================
        //REVERB
        juce::Slider drySlider;
        juce::Slider wetSlider;
        juce::Slider roomSizeSlider;
        juce::Slider dampingSlider;
        juce::Slider widthSlider;
    
        juce::Label dryLabel     { "Dry", "Dry" };
        juce::Label wetLabel    { "Wet", "Wet" };
        juce::Label roomSizeLabel    { "Room Size", "Room Size" };
        juce::Label dampingLabel    { "Damping", "Damping" };
        juce::Label widthLabel    { "Width", "Width" };
    
        
public:
    //==============================================================================
        //FILTER
        std::unique_ptr<juce::SliderParameterAttachment> cutoffSliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> resonanceSliderAttachment;
    //==============================================================================
        //REVERB
        std::unique_ptr<juce::SliderParameterAttachment> drySliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> wetSliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> roomSizeSliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> dampingSliderAttachment;
        std::unique_ptr<juce::SliderParameterAttachment> widthSliderAttachment;

    TheBathroomEffectv3AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheBathroomEffectv3AudioProcessorEditor)
};
