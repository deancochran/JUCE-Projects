#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 500);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{


    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::skyblue);
    
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::TheBathroomEffectBackground_jpg, BinaryData::TheBathroomEffectBackground_jpgSize);
    g.drawImageAt(background, 0, 0);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
