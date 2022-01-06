// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "Gear.hpp"
#include "SimpleImageLayer.hpp"
#include "Layer.hpp"

namespace dang
{
    SimpleImageLayer::SimpleImageLayer(std::shared_ptr<Imagesheet>& is) : Layer(dang::Layer::LT_UNDEFINED)
    {
        _backgroundColour = blit::Pen(255, 255, 255, 255);
        _foregroundColour = blit::Pen(0, 0, 0, 255);
        _highlightColour = blit::Pen(221, 107, 6, 255);

        // paint houses^h^h^h clear the background
        blit::screen.pen = _backgroundColour;
        _displayRect = blit::Rect(0, 0, blit::screen.bounds.w, blit::screen.bounds.h);
        blit::screen.rectangle(_displayRect);

        // get picture for the border
        _is = is;

        _dp.x = (_displayRect.w - _is->getImagesheetSizeW()) / 2;
        _dp.y = (_displayRect.h - _is->getImagesheetSizeH()) / 2;
    }

    SimpleImageLayer::~SimpleImageLayer()
    {
    }

    void SimpleImageLayer::update(uint32_t dt, const dang::Gear &gear)
    {

    }

    void SimpleImageLayer::render(const dang::Gear& gear)
    {
        // clear screen
        blit::screen.pen = _backgroundColour;
        blit::screen.rectangle(_displayRect);

        blit::Rect sr = _is->getBlitRect(0);

        blit::screen.blit(_is->getSurface(), sr, _dp, 0);
    }
}
