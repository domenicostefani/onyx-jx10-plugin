/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
// #include "laf.h"

//==============================================================================
JX10Editor::JX10Editor (JX10AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (ORIGIN_WIDTH, ORIGIN_HEIGHT);

    // Resizeable with fixed aspect ratio
    // setResizeLimits(650, 420, 650, 420);
    setResizable(true, true);

    
    constrainer.setFixedAspectRatio(ASPECT_RATIO);
    // Set minimum and maximum sizes if needed
    constrainer.setMinimumWidth(ORIGIN_WIDTH);
    constrainer.setMinimumHeight(ORIGIN_HEIGHT);
    constrainer.setMaximumWidth(ORIGIN_WIDTH * 4);
    constrainer.setMaximumHeight(ORIGIN_HEIGHT * 4);
    setConstrainer(&constrainer);
    
    // juce::File back_svg_file = juce::File("C:/Users/cimil/Develop/ONYX/onyx-jx10/Source/Data/backing.svg");
    // background_svg_drawable = juce::Drawable::createFromSVGFile(back_svg_file);
    std::unique_ptr<juce::XmlElement> xml_background_svg = juce::XmlDocument::parse(BinaryData::backing_svg); // GET THE SVG AS A XML
    background_svg_drawable = juce::Drawable::createFromSVG(*xml_background_svg);


    addAndMakeVisible (&osc2mix_sld);
    osc2mix_sld.setRange (0, 10, 0.1);
    osc2mix_sld.setValue (0);
    osc2mix_sld.setSliderStyle (juce::Slider::LinearVertical);
    osc2mix_sld.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    osc2mix_sld.setLookAndFeel(&_JX10LookAndFeel);
    driveAttachment.reset(new SliderAttachment(audioProcessor.apvts, "OSC2 Mix", osc2mix_sld));

    addAndMakeVisible (&noise_sld);
    noise_sld.setRange (0, 4, 1);
    noise_sld.setValue (4);
    noise_sld.setSliderStyle (juce::Slider::LinearVertical);
    noise_sld.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    noise_sld.setLookAndFeel(&_JX10LookAndFeel);
    depthAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Noise", noise_sld));

    addAndMakeVisible (&vcfFreq_sld);
    vcfFreq_sld.setRange (0, 4, 1);
    vcfFreq_sld.setValue (4);
    vcfFreq_sld.setSliderStyle (juce::Slider::LinearVertical);
    vcfFreq_sld.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    vcfFreq_sld.setLookAndFeel(&_JX10LookAndFeel);
    srAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Freq", vcfFreq_sld));


    addAndMakeVisible (&vcfRes_sld);
    vcfRes_sld.setRange (0, 100, 0.1);
    vcfRes_sld.setValue (100);
    vcfRes_sld.setSliderStyle (juce::Slider::LinearVertical);
    vcfRes_sld.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    vcfRes_sld.setLookAndFeel(&_JX10LookAndFeel);
    mixAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Reso", vcfRes_sld));

    addAndMakeVisible (&glideMode_sld);
    glideMode_sld.setRange (0, 2, 1);
    glideMode_sld.setValue (0);
    glideMode_sld.setSliderStyle (juce::Slider::LinearVertical);
    glideMode_sld.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    glideMode_sld.setLookAndFeel(&_JX10LookAndFeel);
    glideModeAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Mode", glideMode_sld));

    
    addAndMakeVisible(&vcfEnv_sld);
    vcfEnv_sld.setRange(0, 1, 0.01);
    vcfEnv_sld.setValue(0);
    vcfEnv_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfEnv_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfEnv_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfEnvAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Env", vcfEnv_sld));

    addAndMakeVisible(&vcfVel_sld);
    vcfVel_sld.setRange(0, 1, 0.01);
    vcfVel_sld.setValue(0);
    vcfVel_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfVel_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfVel_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfVelAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Vel", vcfVel_sld));

    addAndMakeVisible(&glideRate_sld);
    glideRate_sld.setRange(0, 1, 0.01);
    glideRate_sld.setValue(0);
    glideRate_sld.setSliderStyle(juce::Slider::LinearVertical);
    glideRate_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    glideRate_sld.setLookAndFeel(&_JX10LookAndFeel);
    glideRateAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Gld Rate", glideRate_sld));

    addAndMakeVisible(&glideBend_sld);
    glideBend_sld.setRange(0, 1, 0.01);
    glideBend_sld.setValue(0);
    glideBend_sld.setSliderStyle(juce::Slider::LinearVertical);
    glideBend_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    glideBend_sld.setLookAndFeel(&_JX10LookAndFeel);
    glideBendAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Gld Bend", glideBend_sld));

    addAndMakeVisible(&lfoRate_sld);
    lfoRate_sld.setRange(0, 10, 0.01);
    lfoRate_sld.setValue(0);
    lfoRate_sld.setSliderStyle(juce::Slider::LinearVertical);
    lfoRate_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lfoRate_sld.setLookAndFeel(&_JX10LookAndFeel);
    lfoRateAttachment.reset(new SliderAttachment(audioProcessor.apvts, "LFO Rate", lfoRate_sld));

    addAndMakeVisible(&lfoAmt_sld);
    lfoAmt_sld.setRange(0, 1, 0.01);
    lfoAmt_sld.setValue(0);
    lfoAmt_sld.setSliderStyle(juce::Slider::LinearVertical);
    lfoAmt_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lfoAmt_sld.setLookAndFeel(&_JX10LookAndFeel);
    lfoAmtAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF LFO", lfoAmt_sld));

    addAndMakeVisible(&vibratoAmt_sld);
    vibratoAmt_sld.setRange(0, 1, 0.01);
    vibratoAmt_sld.setValue(0);
    vibratoAmt_sld.setSliderStyle(juce::Slider::LinearVertical);
    vibratoAmt_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vibratoAmt_sld.setLookAndFeel(&_JX10LookAndFeel);
    vibratoAmtAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Vibrato", vibratoAmt_sld));

    addAndMakeVisible(&vcfEnvA_sld);
    vcfEnvA_sld.setRange(0, 1, 0.01);
    vcfEnvA_sld.setValue(0);
    vcfEnvA_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfEnvA_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfEnvA_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfEnvAAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Att", vcfEnvA_sld));

    addAndMakeVisible(&vcfEnvD_sld);
    vcfEnvD_sld.setRange(0, 1, 0.01);
    vcfEnvD_sld.setValue(0);
    vcfEnvD_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfEnvD_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfEnvD_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfEnvDAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Dec", vcfEnvD_sld));

    addAndMakeVisible(&vcfEnvS_sld);
    vcfEnvS_sld.setRange(0, 1, 0.01);
    vcfEnvS_sld.setValue(0);
    vcfEnvS_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfEnvS_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfEnvS_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfEnvSAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Sus", vcfEnvS_sld));

    addAndMakeVisible(&vcfEnvR_sld);
    vcfEnvR_sld.setRange(0, 1, 0.01);
    vcfEnvR_sld.setValue(0);
    vcfEnvR_sld.setSliderStyle(juce::Slider::LinearVertical);
    vcfEnvR_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    vcfEnvR_sld.setLookAndFeel(&_JX10LookAndFeel);
    vcfEnvRAttachment.reset(new SliderAttachment(audioProcessor.apvts, "VCF Rel", vcfEnvR_sld));

    addAndMakeVisible(&ampEnvA_sld);
    ampEnvA_sld.setRange(0, 1, 0.01);
    ampEnvA_sld.setValue(0);
    ampEnvA_sld.setSliderStyle(juce::Slider::LinearVertical);
    ampEnvA_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampEnvA_sld.setLookAndFeel(&_JX10LookAndFeel);
    ampEnvAAttachment.reset(new SliderAttachment(audioProcessor.apvts, "ENV Att", ampEnvA_sld));

    addAndMakeVisible(&ampEnvD_sld);
    ampEnvD_sld.setRange(0, 1, 0.01);
    ampEnvD_sld.setValue(0);
    ampEnvD_sld.setSliderStyle(juce::Slider::LinearVertical);
    ampEnvD_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampEnvD_sld.setLookAndFeel(&_JX10LookAndFeel);
    ampEnvDAttachment.reset(new SliderAttachment(audioProcessor.apvts, "ENV Dec", ampEnvD_sld));

    addAndMakeVisible(&ampEnvS_sld);
    ampEnvS_sld.setRange(0, 1, 0.01);
    ampEnvS_sld.setValue(0);
    ampEnvS_sld.setSliderStyle(juce::Slider::LinearVertical);
    ampEnvS_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampEnvS_sld.setLookAndFeel(&_JX10LookAndFeel);
    ampEnvSAttachment.reset(new SliderAttachment(audioProcessor.apvts, "ENV Sus", ampEnvS_sld));

    addAndMakeVisible(&ampEnvR_sld);
    ampEnvR_sld.setRange(0, 1, 0.01);
    ampEnvR_sld.setValue(0);
    ampEnvR_sld.setSliderStyle(juce::Slider::LinearVertical);
    ampEnvR_sld.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampEnvR_sld.setLookAndFeel(&_JX10LookAndFeel);
    ampEnvRAttachment.reset(new SliderAttachment(audioProcessor.apvts, "ENV Rel", ampEnvR_sld));



    // Rotary sliders

    juce::Slider::RotaryParameters rotaryParams;
    // rotaryParams.startAngleRadians = MathConstants<float>::pi * 5.0f / 4.0f; // 225 degrees
    // rotaryParams.endAngleRadians = MathConstants<float>::pi * 6.0f / 4.0f;   // -45 degrees
    rotaryParams.startAngleRadians = MathConstants<float>::pi * 5.0f / 4.0f; // 225 degrees
    rotaryParams.endAngleRadians = MathConstants<float>::pi * 11.0f / 4.0f;   // -45 degrees
    rotaryParams.stopAtEnd = true;


    addAndMakeVisible (&octave_knob);
    octave_knob.setRange (0, 4, 1); //TODO: check range
    octave_knob.setValue (0);
    octave_knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    octave_knob.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    octave_knob.setRotaryParameters(rotaryParams);
    octave_knob.setLookAndFeel(&_JX10LookAndFeel);
    octaveAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Octave", octave_knob));

    // Rotary sliders initialization

    addAndMakeVisible(&osc2tune_knob);
    osc2tune_knob.setRange(-24, 24, 1); // Example range for semitone tuning
    osc2tune_knob.setValue(0);
    osc2tune_knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2tune_knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    osc2tune_knob.setRotaryParameters(rotaryParams);
    osc2tune_knob.setLookAndFeel(&_JX10LookAndFeel);
    osc2tuneAttachment.reset(new SliderAttachment(audioProcessor.apvts, "OSC Tune", osc2tune_knob));

    addAndMakeVisible(&osc2fine_knob);
    osc2fine_knob.setRange(-1.0, 1.0, 0.01); // Example range for fine tuning
    osc2fine_knob.setValue(0);
    osc2fine_knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    osc2fine_knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    osc2fine_knob.setRotaryParameters(rotaryParams);
    osc2fine_knob.setLookAndFeel(&_JX10LookAndFeel);
    osc2fineAttachment.reset(new SliderAttachment(audioProcessor.apvts, "OSC Fine", osc2fine_knob));

    addAndMakeVisible(&tuning_knob);
    tuning_knob.setRange(-50, 50, 1); // Example range for global tuning (cents)
    tuning_knob.setValue(0);
    tuning_knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tuning_knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    tuning_knob.setRotaryParameters(rotaryParams);
    tuning_knob.setLookAndFeel(&_JX10LookAndFeel);
    tuningAttachment.reset(new SliderAttachment(audioProcessor.apvts, "Tuning", tuning_knob));

    addAndMakeVisible(&currentProgram);
    auto programIndex = audioProcessor.getCurrentProgram();
    auto programName = audioProcessor.getProgramName(programIndex);
    currentProgram.setText(programName, juce::dontSendNotification);
    currentProgram.setJustificationType(juce::Justification::centred);
    currentProgram.setLookAndFeel(&_JX10LookAndFeel);
    currentProgram.setFont(_JX10LookAndFeel.mainFont.withHeight(21.0f));

    addAndMakeVisible(&sllinkStatus);
    sllinkStatus.setText("Disconnected", juce::dontSendNotification);
    sllinkStatus.setJustificationType(juce::Justification::centred);
    sllinkStatus.setLookAndFeel(&_JX10LookAndFeel);
    sllinkStatus.setFont(_JX10LookAndFeel.mainFont.withHeight(21.0f));

    addAndMakeVisible(programButton);
    programButton.setButtonText("Program---");
    programButton.setLookAndFeel(&invisibleButtonLaF);
    programButton.onClick = [this]() { showFileMenu(&programButton); };

    Timer::startTimerHz(25); // 25 Hz update rate for Program change to update label
}


JX10Editor::~JX10Editor()
{
    // Reset attachments
    driveAttachment.reset();
    depthAttachment.reset();
    srAttachment.reset();
    mixAttachment.reset();
    glideModeAttachment.reset();
    vcfEnvAttachment.reset();
    vcfVelAttachment.reset();
    glideRateAttachment.reset();
    glideBendAttachment.reset();
    lfoRateAttachment.reset();
    lfoAmtAttachment.reset();
    vibratoAmtAttachment.reset();
    vcfEnvAAttachment.reset();
    vcfEnvDAttachment.reset();
    vcfEnvSAttachment.reset();
    vcfEnvRAttachment.reset();
    ampEnvAAttachment.reset();
    ampEnvDAttachment.reset();
    ampEnvSAttachment.reset();
    ampEnvRAttachment.reset();
    octaveAttachment.reset();
    osc2tuneAttachment.reset();
    osc2fineAttachment.reset();
    tuningAttachment.reset();
}
//==============================================================================
void JX10Editor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto lb = getLocalBounds();
    background_svg_drawable->drawWithin(
        g, 
        lb.toFloat(), 
        juce::RectanglePlacement::fillDestination
        | juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yTop,
        1.0f
    );

    auto area = getLocalBounds();
    osc2mix_sld.setBounds(juce::Rectangle<int>(151/2 ,210/2, 18, 144)); // Values from pixel value in the Inkscape file
    noise_sld.setBounds(juce::Rectangle<int>  (471/2 ,210/2, 18, 144));
    vcfFreq_sld.setBounds(juce::Rectangle<int>(760/2 ,210/2, 18, 144));
    vcfRes_sld.setBounds(juce::Rectangle<int> (1094/2,210/2, 18, 144));
    glideMode_sld.setBounds(juce::Rectangle<int>(1780/2,211/2, 18, 144));
    vcfEnv_sld.setBounds(    juce::Rectangle<int>(774/2,601/2, 18, 144));
    vcfVel_sld.setBounds(    juce::Rectangle<int>(1094/2,601/2, 18, 144));
    glideRate_sld.setBounds( juce::Rectangle<int>(1620/2,601/2, 18, 144));
    glideBend_sld.setBounds( juce::Rectangle<int>(1750/2,601/2, 18, 144));
    lfoRate_sld.setBounds(   juce::Rectangle<int>(124/2,1060/2, 18, 144));
    lfoAmt_sld.setBounds(    juce::Rectangle<int>(324/2,1060/2, 18, 144));
    vibratoAmt_sld.setBounds(juce::Rectangle<int>(548/2,1060/2, 18, 144));
    vcfEnvA_sld.setBounds(   juce::Rectangle<int>(815/2,1060/2, 18, 144));
    vcfEnvD_sld.setBounds(   juce::Rectangle<int>(943/2,1060/2, 18, 144));
    vcfEnvS_sld.setBounds(   juce::Rectangle<int>(1072/2,1060/2, 18, 144));
    vcfEnvR_sld.setBounds(   juce::Rectangle<int>(1202/2,1060/2, 18, 144));
    ampEnvA_sld.setBounds(   juce::Rectangle<int>(1452/2,1060/2, 18, 144));
    ampEnvD_sld.setBounds(   juce::Rectangle<int>(1580/2,1060/2, 18, 144));
    ampEnvS_sld.setBounds(   juce::Rectangle<int>(1709/2,1060/2, 18, 144));
    ampEnvR_sld.setBounds(   juce::Rectangle<int>(1839/2,1060/2, 18, 144));

    
    octave_knob.setBounds(juce::Rectangle<int> (1354/2, 320/2, 24, 24));
    osc2tune_knob.setBounds(juce::Rectangle<int> (143/2, 714/2, 24, 24));
    osc2fine_knob.setBounds(juce::Rectangle<int> (463/2, 714/2, 24, 24));
    tuning_knob.setBounds(juce::Rectangle<int> (1354/2, 714/2, 24, 24));

    sllinkStatus.setBounds(juce::Rectangle<int>(2007/2, 1196/2, 372/2, 60/2));

    currentProgram.setBounds(juce::Rectangle<int>(874/2, 1463/2, 1530/2, 60/2));
    programButton.setBounds(juce::Rectangle<int>(74/2, 1463/2, 2330/2, 60/2));


    // Calculate scale factors
    float scaleX = area.getWidth() / (float)ORIGIN_WIDTH;
    float scaleY = area.getHeight() / (float)ORIGIN_HEIGHT;
    
    // Use the smaller scale factor to maintain aspect ratio
    float scale = juce::jmin(scaleX, scaleY);
    
    // Center the scaled content
    float scaledWidth = ORIGIN_WIDTH * scale;
    float scaledHeight = ORIGIN_HEIGHT * scale;
    float xOffset = (area.getWidth() - scaledWidth) * 0.5f;
    float yOffset = (area.getHeight() - scaledHeight) * 0.5f;
    
    // Create and apply transform
    juce::AffineTransform transform = juce::AffineTransform::scale(scale)
                                        .translated(xOffset, yOffset);

    // Apply the transform to components
    osc2mix_sld.setTransform(transform);
    noise_sld.setTransform(transform);
    vcfFreq_sld.setTransform(transform);
    vcfRes_sld.setTransform(transform);
    glideMode_sld.setTransform(transform);
    vcfEnv_sld.setTransform(transform);
    vcfVel_sld.setTransform(transform);
    glideRate_sld.setTransform(transform);
    glideBend_sld.setTransform(transform);
    lfoRate_sld.setTransform(transform);
    lfoAmt_sld.setTransform(transform);
    vibratoAmt_sld.setTransform(transform);
    vcfEnvA_sld.setTransform(transform);
    vcfEnvD_sld.setTransform(transform);
    vcfEnvS_sld.setTransform(transform);
    vcfEnvR_sld.setTransform(transform);
    ampEnvA_sld.setTransform(transform);
    ampEnvD_sld.setTransform(transform);
    ampEnvS_sld.setTransform(transform);
    ampEnvR_sld.setTransform(transform);
    //
    octave_knob.setTransform(transform);
    osc2tune_knob.setTransform(transform);
    osc2fine_knob.setTransform(transform);
    tuning_knob.setTransform(transform);
    //
    currentProgram.setTransform(transform);
    sllinkStatus.setTransform(transform);
    programButton.setTransform(transform);
}

void JX10Editor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // driveSlider->setBounds(0, 0, 50, getHeight());
    // driveLabel->setBounds(0, getHeight() - 20, getWidth() / 3, 20);
    repaint();
    
}

void JX10Editor::timerCallback()
{
    auto programIndex = audioProcessor.getCurrentProgram();
    auto programName = audioProcessor.getProgramName(programIndex);
    currentProgram.setText(programName, juce::dontSendNotification);
}

void JX10Editor::showFileMenu(juce::TextButton* button)
{
    if (button == &programButton)
    {
        // Handle file button actions
        juce::PopupMenu menu;
        menu.setLookAndFeel(&_JX10LookAndFeel);
        // Set colour black on white

        // menu.addItem(1, "Save Preset");
        // menu.addItem(2, "Load Preset");
        // menu.addItem(1, "Program");
        // menu.addSeparator();
        // menu.addItem(3, "Info");

        for (int i = 0; i < audioProcessor.getNumPrograms(); ++i) {
            menu.addItem(i + 1, audioProcessor.getProgramName(i));
        }

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&programButton),
            [this](int result) {
                if (result > 0 && result <= audioProcessor.getNumPrograms()) {
                    audioProcessor.setCurrentProgram(result - 1);
                }
            });


        //         if (result == 1) {
        //             // Handle Save Preset - Async
        //             auto fileChooser = std::make_shared<juce::FileChooser>("Save Preset", juce::File(), "*.preset");
        //             fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
        //                 [this, fileChooser](const juce::FileChooser& chooser) {
        //                     auto file = chooser.getResult();
        //                     if (file != juce::File{}) {
        //                         juce::MemoryBlock mb;
        //                         audioProcessor.getStateInformation(mb);
        //                         file.replaceWithData(mb.getData(), mb.getSize());
        //                     }
        //                 });
        //         }
        //         else if (result == 2) {
        //             // Handle Load Preset - Async
        //             auto fileChooser = std::make_shared<juce::FileChooser>("Load Preset", juce::File(), "*.preset");
        //             fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        //                 [this, fileChooser](const juce::FileChooser& chooser) {
        //                     auto file = chooser.getResult();
        //                     if (file != juce::File{}) {
        //                         juce::MemoryBlock mb;
        //                         if (file.loadFileAsData(mb)) {
        //                             audioProcessor.setStateInformation(mb.getData(), static_cast<int>(mb.getSize()));
        //                             this->repaint();
        //                         }
        //                     }
        //                 });
        //         }
        //         else if (result == 3) {
        //             // Handle Info
        //             openInfoURL();
        //         }
        //     });
    }
    // else if (button == &editButton)
    // {
    //     // Handle edit button actions
    //     juce::PopupMenu menu;
    //     menu.setLookAndFeel(&degradeLookAndFeel);
    //     menu.addItem(1, "Undo");
    //     menu.addItem(2, "Redo");

    //     menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&editButton),
    //         [this](int result) {
    //             if (result == 1) {
    //                 // Handle Undo
    //                 audioProcessor.performUndo();
    //             }
    //             else if (result == 2) {
    //                 // Handle Redo
    //                 audioProcessor.performRedo();
    //             }
    //         });
    // }
    // else if (button == &viewButton)
    // {
    //     // Handle view button actions
    //     juce::PopupMenu menu;
    //     menu.setLookAndFeel(&degradeLookAndFeel);
    //     menu.addItem(1, "Zoom In");
    //     menu.addItem(2, "Zoom Out");
    //     menu.addItem(3, "Reset View");

    //     menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&viewButton),
    //         [this](int result) {
    //             if (result == 1) {
    //                 // Handle Zoom In by increasing the size of the plugin window
    //                 float increase = 0.1f; // Increase size by 10%
    //                 this->setSize(
    //                     static_cast<int>(getWidth() * (1.0f + increase)),
    //                     static_cast<int>(getHeight() * (1.0f + increase))
    //                 );
    //             }
    //             else if (result == 2) {
    //                 // Handle Zoom Out
    //                 float decrease = 0.1f; // Decrease size by 10%
    //                 this->setSize(
    //                     static_cast<int>(getWidth() * (1.0f - decrease)),
    //                     static_cast<int>(getHeight() * (1.0f - decrease))
    //                 );
    //             }
    //             else if (result == 3) {
    //                 // Handle Reset View
    //                 this->setSize(ORIGIN_WIDTH, ORIGIN_HEIGHT); // Reset to original size
    //             }
    //         });
    // }
    // else if (button == &specialButton)
    // {
    //     // Handle special button actions
    //     juce::PopupMenu menu;
    //     menu.setLookAndFeel(&degradeLookAndFeel);
    //     menu.addItem(1, "You are special ;)");

    //     menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&specialButton),
    //         [this](int result) {
    //             if (result == 1) {
                    
    //             }
    //         });
    // }
}