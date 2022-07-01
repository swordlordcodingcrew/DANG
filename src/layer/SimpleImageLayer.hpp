// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework
#pragma once

#include <list>

#include "Imagesheet.hpp"
#include "src/layer/Layer.hpp"
#include "32blit.hpp"

namespace dang
{
class SimpleImageLayer : public dang::Layer
    {
    public:
        SimpleImageLayer(std::shared_ptr<Imagesheet>& is);
        ~SimpleImageLayer() override;

        void    update(uint32_t dt, const dang::Gear& gear) override;
        void    render(const dang::Gear& gear) override;

    protected:
        explicit SimpleImageLayer(Layer::E_TYPE type) : Layer(type) {};

        std::shared_ptr<dang::Imagesheet> _is;

        blit::Point _dp = {0, 0};

        blit::Pen _backgroundColour;
        blit::Pen _foregroundColour;
        blit::Pen _highlightColour;

        blit::Rect _displayRect;
    };
}

