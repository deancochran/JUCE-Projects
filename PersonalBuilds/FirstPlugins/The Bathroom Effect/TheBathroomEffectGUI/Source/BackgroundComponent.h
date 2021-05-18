/*
  ==============================================================================

    BackgroundComponent.h
    Created: 1 Feb 2021 3:18:20pm
    Author:  Dean Cochran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BackgroundComponent  : public juce::Component
{
public:
    BackgroundComponent();
    ~BackgroundComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackgroundComponent)
};
