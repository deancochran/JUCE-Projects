/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheBathroomEffectv0_1AudioProcessorEditor::TheBathroomEffectv0_1AudioProcessorEditor (TheBathroomEffectv0_1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const auto boxWidth  = 100;
    const auto boxHeight = 20;
    
    //======FILTER==================================================================
    //==============================================================================
    
    cutoffSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    cutoffSlider.setRange(0.0f, 2000.0f);
    cutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    cutoffSlider.setTextValueSuffix(" Hz");
    
    cutoffLabel.setText("Frequency", juce::dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    
    addAndMakeVisible (cutoffSlider);
    addAndMakeVisible(cutoffLabel);
    
    //=============================================================================
    resonanceSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    resonanceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    resonanceSlider.setRange(0.0f, 1.0f);
    resonanceSlider.setTextValueSuffix(" %");
    
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    resonanceLabel.attachToComponent(&resonanceSlider, false);
    
    addAndMakeVisible (resonanceSlider);
    addAndMakeVisible (resonanceLabel);
    
    //==============================================================================
    cutoffSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("CUTOFF"), cutoffSlider);
    resonanceSliderAttachment     = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("RESONANCE"), resonanceSlider);
    //==============================================================================
    //==============================================================================
    
    //======REVERB==================================================================
//    drySlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
//    drySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
//    drySlider.setRange(0.0f, 1.0f);
//    drySlider.setTextValueSuffix(" %");
//
//    dryLabel.setText("Dry Level", juce::dontSendNotification);
//    dryLabel.attachToComponent(&drySlider, false);
//
//    addAndMakeVisible (drySlider);
//    addAndMakeVisible(dryLabel);
//    //==============================================================================
//    wetSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
//    wetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
//    wetSlider.setRange(0.0f, 1.0f);
//    wetSlider.setTextValueSuffix(" %");
//
//    wetLabel.setText("Dry Level", juce::dontSendNotification);
//    wetLabel.attachToComponent(&wetSlider, false);
//
//    addAndMakeVisible (wetSlider);
//    addAndMakeVisible(wetLabel);
//    //==============================================================================
//    drySliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("DRY"), drySlider);
//    wetSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("WET"), wetSlider);
    //==============================================================================
    
    
    setSize (800, 500);
}

TheBathroomEffectv0_1AudioProcessorEditor::~TheBathroomEffectv0_1AudioProcessorEditor()
{
}

//==============================================================================
void TheBathroomEffectv0_1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::slategrey);
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TheBathroomEffectv0_1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int border = 20;
    int width = 200;
    int height = 200;
    
    //#1 cutoff
    cutoffSlider.setBounds (border, border, width-border, height-border);
    
    //#2 resonance
    resonanceSlider.setBounds (border, 250, width-border, height-border);
    
    //#3 width
    //.setBounds (200, border, width, height);
    
    //#4 free mode
    //.setBounds (200, 250, width, height-border);
    
    //#5 room size
    //.setBounds (400, border, width, height);
    
    //#6 damp
    //.setBounds (400, 250, width, height-border);
    
    //#7 dry
    //drySlider.setBounds (600, border, width-border, height-border);
    
    //#8 wet
    //wetSlider.setBounds (600, 250, width-border, height-border);

}
