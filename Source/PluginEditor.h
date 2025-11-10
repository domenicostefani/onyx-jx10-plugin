/*
  Degrade - OnyxDSP
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "laf.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


//==============================================================================
/**
*/
class JX10Editor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    JX10Editor (JX10AudioProcessor&);
    ~JX10Editor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    void showFileMenu(juce::TextButton*);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JX10AudioProcessor& audioProcessor;

    std::unique_ptr<juce::Drawable> background_svg_drawable;
    juce::ComponentBoundsConstrainer constrainer;

    const int ORIGIN_WIDTH = 2440/2;
    const int ORIGIN_HEIGHT = 1550/2;	
    const float ASPECT_RATIO = (float)ORIGIN_WIDTH/ORIGIN_HEIGHT;

    JX10LookAndFeel _JX10LookAndFeel;

    // Linear sliders
    juce::Slider osc2mix_sld{ "osc2mix_sld"},
                 noise_sld{ "noise_sld"},
                 vcfFreq_sld{ "vcfFreq_sld"},
                 vcfRes_sld{ "vcfRes_sld"},
                 glideMode_sld{ "glideMode_sld"},
                 vcfEnv_sld{ "vcfEnv_sld"},
                 vcfVel_sld{ "vcfVel_sld"},
                 glideRate_sld{ "glideRate_sld"},
                 glideBend_sld{ "glideBend_sld"},
                 lfoRate_sld{ "lfoRate_sld"},
                 lfoAmt_sld{ "lfoAmt_sld"},
                 vibratoAmt_sld{ "vibratoAmt_sld"},
                 vcfEnvA_sld{ "vcfEnvA_sld"},
                 vcfEnvD_sld{ "vcfEnvD_sld"},
                 vcfEnvS_sld{ "vcfEnvS_sld"},
                 vcfEnvR_sld{ "vcfEnvR_sld"},
                 ampEnvA_sld{ "ampEnvA_sld"},
                 ampEnvD_sld{ "ampEnvD_sld"},
                 ampEnvS_sld{ "ampEnvS_sld"},
                 ampEnvR_sld{ "ampEnvR_sld"};
    std::unique_ptr<SliderAttachment> driveAttachment,
                                      depthAttachment,
                                      srAttachment,
                                      mixAttachment,
                                      glideModeAttachment,
                                      vcfEnvAttachment,
                                      vcfVelAttachment,
                                      glideRateAttachment,
                                      glideBendAttachment,
                                      lfoRateAttachment,
                                      lfoAmtAttachment,
                                      vibratoAmtAttachment,
                                      vcfEnvAAttachment,
                                      vcfEnvDAttachment,
                                      vcfEnvSAttachment,
                                      vcfEnvRAttachment,
                                      ampEnvAAttachment,
                                      ampEnvDAttachment,
                                      ampEnvSAttachment,
                                      ampEnvRAttachment;
                                    
    // Rotary sliders
    juce::Slider octave_knob{ "octave_knob"},
                 osc2tune_knob{ "osc2tune_knob"},
                 osc2fine_knob{ "osc2fine_knob"},
                 tuning_knob{ "tuning_knob"};
    std::unique_ptr<SliderAttachment> octaveAttachment,
                                      osc2tuneAttachment,
                                      osc2fineAttachment,
                                      tuningAttachment;

    juce::Label currentProgram, sllinkStatus;
    juce::TextButton programButton;
    InvisibleTextButtonLookAndFeel invisibleButtonLaF;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JX10Editor)
};

