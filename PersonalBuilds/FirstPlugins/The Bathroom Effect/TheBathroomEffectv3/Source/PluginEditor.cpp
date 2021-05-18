/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TheBathroomEffectv3AudioProcessorEditor::TheBathroomEffectv3AudioProcessorEditor (TheBathroomEffectv3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const auto boxWidth  = 100;
    const auto boxHeight = 20;
    
    //======FILTER==================================================================
    //==============================================================================
        
    cutoffSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    cutoffSlider.setRange(20.0f, 2000.0f);
    cutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    cutoffSlider.setPopupDisplayEnabled(true, true, this);
    cutoffSlider.setTextValueSuffix(" Hz");
    
    cutoffLabel.setText("Frequency Cutoff", juce::dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    
    addAndMakeVisible (cutoffSlider);
    addAndMakeVisible(cutoffLabel);
    
    //=============================================================================
    resonanceSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    resonanceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    resonanceSlider.setRange(0.0f, 1.0f);
    resonanceSlider.setPopupDisplayEnabled(true, true, this);
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
    drySlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    drySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    drySlider.setRange(0.0f, 1.0f);
    drySlider.setPopupDisplayEnabled(true, true, this);
    drySlider.setTextValueSuffix(" %");

    dryLabel.setText("Dry Level", juce::dontSendNotification);
    dryLabel.attachToComponent(&drySlider, false);

    addAndMakeVisible (drySlider);
    addAndMakeVisible(dryLabel);
    //==============================================================================
    wetSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    wetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    wetSlider.setRange(0.0f, 1.0f);
    wetSlider.setPopupDisplayEnabled(true, true, this);
    wetSlider.setTextValueSuffix(" %");

    wetLabel.setText("Wet Level", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetSlider, false);

    addAndMakeVisible (wetSlider);
    addAndMakeVisible(wetLabel);
    //==============================================================================
    roomSizeSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    roomSizeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    roomSizeSlider.setRange(0.0f, 1.0f);
    roomSizeSlider.setPopupDisplayEnabled(true, true, this);
    roomSizeSlider.setTextValueSuffix(" %");

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);

    addAndMakeVisible (roomSizeSlider);
    addAndMakeVisible(roomSizeLabel);
    //==============================================================================
    dampingSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    dampingSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    dampingSlider.setRange(0.0f, 1.0f);
    dampingSlider.setPopupDisplayEnabled(true, true, this);
    dampingSlider.setTextValueSuffix(" %");

    dampingLabel.setText("Dampening Level", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, false);

    addAndMakeVisible (dampingSlider);
    addAndMakeVisible(dampingLabel);
    //==============================================================================
    widthSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    widthSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, boxWidth, boxHeight);
    widthSlider.setRange(0.0f, 1.0f);
    widthSlider.setPopupDisplayEnabled(true, true, this);
    widthSlider.setTextValueSuffix(" %");

    widthLabel.setText("Stereo Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, false);

    addAndMakeVisible (widthSlider);
    addAndMakeVisible(widthLabel);
    //==============================================================================
    drySliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("DRY"), drySlider);
    wetSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("WET"), wetSlider);
    roomSizeSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("ROOMSIZE"), roomSizeSlider);
    dampingSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("DAMPING"), dampingSlider);
    widthSliderAttachment        = std::make_unique<juce::SliderParameterAttachment>(*audioProcessor.apvts.getParameter ("WIDTH"), widthSlider);
    //==============================================================================
    //==============================================================================
    

    

    setSize (800, 500);
}

TheBathroomEffectv3AudioProcessorEditor::~TheBathroomEffectv3AudioProcessorEditor()
{
}

//==============================================================================
void TheBathroomEffectv3AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::slategrey);
    
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::TheBathroomEffectBackground_jpg, BinaryData::TheBathroomEffectBackground_jpgSize);
    g.drawImageAt(background, 0, 0);
}

void TheBathroomEffectv3AudioProcessorEditor::resized()
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
    
    //#3 gain or freeze mode
    //.setBounds (200, border, width, height);
    
    //#4 width
    widthSlider.setBounds (200, 250, width, height-border);
    
    //#5 room size
    roomSizeSlider.setBounds (400, border, width, height);
    
    //#6 damping
    dampingSlider.setBounds (400, 250, width, height-border);
    
    //#7 dry
    drySlider.setBounds (600, border, width-border, height-border);
    
    //#8 wet
    wetSlider.setBounds (600, 250, width-border, height-border);
}
