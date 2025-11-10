#pragma once

#include "BinaryData.h"
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <cmath>

class JX10LookAndFeel : public juce::LookAndFeel_V4
{
    std::unique_ptr<juce::Drawable> sldback_svd_drawable, sldcur_svd_drawable;
public:    
    // Use Arial as main font
    // juce::Font mainFont{ "Arial", 20.0f, juce::Font::bold };
    // [build] /Users/domenico/Develop/ONYX/onyx-jx10/Source/laf.h:18:24: warning: 'Font' is deprecated: Use the constructor that takes a FontOptions argument [-Wdeprecated-declarations]
    juce::Font mainFont{ juce::FontOptions("Arial", 20.0f, juce::Font::bold) };

public:
    JX10LookAndFeel()
    {
        std::unique_ptr<juce::XmlElement> xml_sldback_svg = juce::XmlDocument::parse(BinaryData::slider_back_svg); // GET THE SVG AS A XML
        sldback_svd_drawable = juce::Drawable::createFromSVG(*xml_sldback_svg);
        std::unique_ptr<juce::XmlElement> xml_sldcur_svg = juce::XmlDocument::parse(BinaryData::slider_cursor_svg); // GET THE SVG AS A XML
        sldcur_svd_drawable = juce::Drawable::createFromSVG(*xml_sldcur_svg);


        // To debug svg issues quickly, uncomment the lines below so htat the svg files are loaded from the filesystem at each gui render
        // juce::File sldback_svg_file = juce::File("C:/Users/cimil/Develop/ONYX/onyx-jx10/Source/Data/slider_back.svg");
        // sldback_svd_drawable = juce::Drawable::createFromSVGFile(sldback_svg_file);
        // juce::File sldcur_svg_file = juce::File("C:/Users/cimil/Develop/ONYX/onyx-jx10/Source/Data/slider_cursor.svg");
        // sldcur_svd_drawable = juce::Drawable::createFromSVGFile(sldcur_svg_file);

        
        this->setColour(juce::PopupMenu::backgroundColourId, juce::Colours::white);
        this->setColour(juce::PopupMenu::textColourId, juce::Colours::black);

        this->setColour(juce::Slider::thumbColourId, juce::Colours::white);
        // this->setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        this->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::black);
        this->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
    }

    /**
     * tHIS SETS the offset from top and bottom for both the thumb and background drawn in drawLinearSlider
     */
    int getSliderThumbRadius (Slider& slider) override
    {
        return static_cast<int> (slider.getHeight() / 10.0f); // This is 7 because the initial svg has a 7 step scale and that is also the top and bottom margin.
    }

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                        float sliderPos,
                                        float minSliderPos,
                                        float maxSliderPos,
                                        const Slider::SliderStyle style, 
                                        Slider& slider) override
    {
        float backgroundBottomMargin = (float)getSliderThumbRadius(slider); //roundf(0.124 * height); //0.07f
        float backgroundTopMargin    = backgroundBottomMargin;

        if (slider.isBar())
        {
            g.setColour (slider.findColour (Slider::trackColourId));
            g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                            : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));

            drawLinearSliderOutline (g, x, y, width, height, style, slider);
        }
        else
        {
            auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);


            Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                    slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

            Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                                slider.isHorizontal() ? startPoint.y : (float) y);

            // Draw background svg
            if (sldback_svd_drawable != nullptr)
            {
                // With cast to all values
                juce::Rectangle<float> svgBackRect = juce::Rectangle<float>(static_cast<float> (x), 
                                                     static_cast<float> (y) - backgroundTopMargin, 
                                                     static_cast<float> (width), 
                                                     static_cast<float> (height) + backgroundTopMargin + backgroundBottomMargin);
                sldback_svd_drawable->drawWithin(g, svgBackRect, juce::RectanglePlacement::stretchToFit , 1.0f);

                // g.setColour(juce::Colours::green);
                // g.drawRect(x, y, width, height);

                // g.setColour(juce::Colours::orange);
                // g.drawRect(svgBackRect);
            }

            Point<float> minPoint, maxPoint, thumbPoint;

            if (isTwoVal || isThreeVal)
            {
                minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                            slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

                if (isThreeVal)
                    thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                                slider.isHorizontal() ? (float) height * 0.5f : sliderPos };

                maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                            slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
                auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

                minPoint = startPoint;
                maxPoint = { kx, ky };
            }

            if (! isTwoVal)
            {
                // g.setColour (slider.findColour (Slider::thumbColourId));
                // g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));

                if (sldcur_svd_drawable != nullptr)
                {
                    // TODO: Now only works for vertical
                    float thumbWidth = static_cast<float> (width);
                    float thumbHeight = thumbWidth / 32.0f * 51.0f; // svg aspect ratio is 32x51


                    // juce::Rectangle<float> svgCurRect(maxPoint.x, maxPoint.y, thumbWidth, thumbHeight);
                    // juce::Rectangle<float> svgCurRect (static_cast<float> (thumbWidth), static_cast<float> (thumbHeight)).withCentre (isThreeVal ? thumbPoint : maxPoint);
                    juce::Rectangle<float> svgCurRect = juce::Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbHeight)).withCentre (isThreeVal ? thumbPoint : maxPoint);
                    sldcur_svd_drawable->drawWithin(g, svgCurRect, juce::RectanglePlacement::stretchToFit , 1.0f);
                }
            }



            if (slider.isBar())
                drawLinearSliderOutline (g, x, y, width, height, style, slider);

        }
    }

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour (Slider::rotarySliderFillColourId);

        auto bounds = Rectangle<int> (x, y, width, height).toFloat();

        // auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        // auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        // auto lineW = jmin (8.0f, radius * 0.5f);
        // auto arcRadius = radius - lineW * 0.5f;

        // Path backgroundArc;
        // backgroundArc.addCentredArc (bounds.getCentreX(),
        //                             bounds.getCentreY(),
        //                             arcRadius,
        //                             arcRadius,
        //                             0.0f,
        //                             rotaryStartAngle,
        //                             rotaryEndAngle,
        //                             true);

        // g.setColour (outline);
        // g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));

        // if (slider.isEnabled())
        // {
        //     Path valueArc;
        //     valueArc.addCentredArc (bounds.getCentreX(),
        //                             bounds.getCentreY(),
        //                             arcRadius,
        //                             arcRadius,
        //                             0.0f,
        //                             rotaryStartAngle,
        //                             toAngle,
        //                             true);

        //     g.setColour (fill);
        //     g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
        // }

        // auto thumbWidth = lineW * 2.0f;
        // Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
        //                         bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));

        // g.setColour (slider.findColour (Slider::thumbColourId));
        // g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));


        // Draw a simple circular knob with an indicator line reaching the center and edge of the circle
        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin (4.0f, radius * 0.2f);
        auto arcRadius = radius - lineW * 0.5f;
        float size = jmin(bounds.getWidth(), bounds.getHeight()) - lineW;
        Rectangle<float> barBounds (bounds.getCentreX() - size * 0.5f,
                                    bounds.getCentreY() - size * 0.5f,
                                    size, size);
        // auto background = slider.findColour (Slider::backgroundColourId);
        g.setColour (fill);
        g.fillEllipse (barBounds);
        g.setColour (outline);
        g.drawEllipse (barBounds, lineW);
        if (slider.isEnabled())
        {
            Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                    bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
            auto thumbColor = slider.findColour (Slider::thumbColourId);
            g.setColour (thumbColor);
            g.drawLine (bounds.getCentreX(), bounds.getCentreY(), thumbPoint.x, thumbPoint.y, lineW);
        }


    }


    juce::Font getPopupMenuFont() override
    {
        // Return your desired font
        return mainFont.withHeight(28.0f); // Adjust the height as needed
        // Or use a custom font:
        // return juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::your_font_data, BinaryData::your_font_size));
    }
    

    void drawPopupMenuBackground (Graphics& g, int width, int height) override
    {
        auto background = findColour (PopupMenu::backgroundColourId);

        g.fillAll (background);
        // g.setColour (background.overlaidWith (Colour (0x2badd8e6)));

        // for (int i = 0; i < height; i += 3)
        //     g.fillRect (0, i, width, 1);

    // #if ! JUCE_MAC
    //     g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
    //     g.drawRect (0, 0, width, height);
    // #endif

        // Draw thick border around the menu with textcolourid
        g.setColour(findColour(PopupMenu::textColourId));
        g.drawRect(0, 0, width, height, 2); //

    }

    void drawPopupMenuBackgroundWithOptions (Graphics& g,
                                                int width,
                                                int height,
                                                const PopupMenu::Options&) override
    {
        drawPopupMenuBackground (g, width, height);
    }
};


