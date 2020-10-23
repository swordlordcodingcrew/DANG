// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <forward_list>

#include "Layer.h"

namespace dang
{
    using spSprite = std::shared_ptr<Sprite>;

    class SpriteLayer : public Layer
    {
    public:
        SpriteLayer();
        ~SpriteLayer() override = default;

        void    update(uint32_t time, uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual void    addSprite(spSprite spr);
        virtual void    removeSprite(spSprite spr);

    protected:
        explicit SpriteLayer(Layer::E_TYPE type) : Layer(type) {};

        std::forward_list<spSprite> _sprites;

    };

}

