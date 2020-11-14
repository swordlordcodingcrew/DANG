// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <list>

#include "Layer.h"

namespace dang
{
    using spSprite = std::shared_ptr<Sprite>;

    class SpriteLayer : public Layer
    {
    public:
        SpriteLayer();
        ~SpriteLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual void    addSprite(spSprite spr);
        virtual void    removeSprite(spSprite spr);

    protected:
        explicit SpriteLayer(Layer::E_TYPE type) : Layer(type) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);
        // sprites inside active area
        std::list<spSprite> _active_sprites;

        // inactive sprites
        std::list<spSprite> _inactive_sprites;

    };

}

