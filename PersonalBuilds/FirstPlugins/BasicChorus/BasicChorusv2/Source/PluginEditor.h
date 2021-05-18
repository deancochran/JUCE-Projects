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
class BasicChorusv2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicChorusv2AudioProcessorEditor (BasicChorusv2AudioProcessor&);
    ~BasicChorusv2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    
     juce::Slider rateSlider;
     juce::Slider depthSlider;
     juce::Slider delaySlider;
     juce::Slider feedbackSlider;
     juce::Slider mixSlider;
     
     juce::Label rateLabel {"Rate","Rate"};
     juce::Label depthLabel {"Depth","Depth"};
     juce::Label delayLabel    { "Delay" , "Delay" };
     juce::Label feedbackLabel { "Feedback", "Feedback" };
     juce::Label mixLabel      { "Mix", "Mix" };
     juce::Label pluginTitle   { "Plug-in Title", "Chorus" };
    
    
    BasicChorusv2AudioProcessor& audioProcessor;
    
public:
    std::unique_ptr<juce::SliderParameterAttachment> rateSliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> depthSliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> delaySliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> feedbackSliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> mixSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicChorusv2AudioProcessorEditor)
};
