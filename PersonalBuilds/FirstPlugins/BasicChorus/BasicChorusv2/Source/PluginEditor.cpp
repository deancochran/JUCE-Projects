/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicChorusv2AudioProcessorEditor::BasicChorusv2AudioProcessorEditor (BasicChorusv2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    using SliderStyle    = juce::Slider::SliderStyle;
    const auto boxWidth  = 90;
    const auto boxHeight = 20;
    
    //RATE SLIDER AND LABEL
    rateSlider.setSliderStyle (SliderStyle::Rotary);
    rateSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, boxWidth, boxHeight);
    rateSlider.setPopupDisplayEnabled(true, true, this);
    rateSlider.setTextValueSuffix(" %");
    addAndMakeVisible (rateSlider);
    
    rateLabel.setJustificationType (juce::Justification::centred);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.attachToComponent(&rateSlider, false);
        
    addAndMakeVisible (rateLabel);
    
    //DEPTH SLIDER AND LABEL
    depthSlider.setSliderStyle (SliderStyle::Rotary);
    depthSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, boxWidth, boxHeight);
    depthSlider.setPopupDisplayEnabled(true, true, this);
    depthSlider.setTextValueSuffix(" %");
    addAndMakeVisible (depthSlider);
    
    depthLabel.setJustificationType (juce::Justification::centred);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.attachToComponent(&depthSlider, false);
    addAndMakeVisible (depthLabel);
    
    //DELAY SLIDER AND LABEL
    delaySlider.setSliderStyle (SliderStyle::Rotary);
    delaySlider.setTextBoxStyle (juce::Slider::NoTextBox, true, boxWidth, boxHeight);
    delaySlider.setPopupDisplayEnabled(true, true, this);
    delaySlider.setTextValueSuffix(" %");
    addAndMakeVisible (delaySlider);
    
    delayLabel.setJustificationType (juce::Justification::centred);
    delayLabel.setText("Delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delaySlider, false);
    addAndMakeVisible (delayLabel);
    
    //FEEDBACK SLIDER AND LABEL
    feedbackSlider.setSliderStyle (SliderStyle::Rotary);
    feedbackSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, boxWidth, boxHeight);
    feedbackSlider.setPopupDisplayEnabled(true, true, this);
    feedbackSlider.setTextValueSuffix(" %");
    addAndMakeVisible (feedbackSlider);
    
    feedbackLabel.setJustificationType (juce::Justification::centred);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    addAndMakeVisible (feedbackLabel);
    
    //MIX SLIDER AND LABEL
    mixSlider.setSliderStyle (SliderStyle::Rotary);
    mixSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, boxWidth, boxHeight);
    mixSlider.setPopupDisplayEnabled(true, true, this);
    mixSlider.setTextValueSuffix(" %");
    addAndMakeVisible (mixSlider);
    
    mixLabel.setJustificationType (juce::Justification::centred);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    addAndMakeVisible (mixLabel);
    
    //SLIDER ATTACHEMENTS
    rateSliderAttachment = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("RATE"), rateSlider);
    depthSliderAttachment = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("DEPTH"), depthSlider);
    delaySliderAttachment = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("DELAY"), delaySlider);
    feedbackSliderAttachment = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("FEEDBACK"), feedbackSlider);
    mixSliderAttachment = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("MIX"), mixSlider);
    
    
    setSize (400, 500);
}

BasicChorusv2AudioProcessorEditor::~BasicChorusv2AudioProcessorEditor()
{
}

//==============================================================================
void BasicChorusv2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (juce::Colours::black);
}

void BasicChorusv2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //#1 RATE
    rateSlider.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.666f);
    
    //#2 DEPTH
    depthSlider.setBoundsRelative(0.0f, 0.666f, 0.25f, 0.333f);
    
    //#3 DELAY
    delaySlider.setBoundsRelative(0.25f, 0.666f, 0.25f, 0.333f);
    
    //#4 FEEDBACK
    feedbackSlider.setBoundsRelative(0.50f, 0.666f, 0.25f, 0.333f);
    
    //#5 MIX
    mixSlider.setBoundsRelative(0.75f, 0.666f, 0.25f, 0.333f);
    
}
