// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 02.03.20.
//

#pragma once

#include "Layer.h"
#include "SweptAABBCollision.h"

namespace dang
{
    class SpriteLayer : public Layer
    {
    public:
        SpriteLayer();
        SpriteLayer(const RectF& layer_size_px);
        virtual ~SpriteLayer();

        virtual void    update(uint32_t time, const Gear& gear) override;
        virtual void    render(const Gear& gear) override;

        void    addSprite(std::shared_ptr<Sprite> spr);
        void    removeSprite(std::shared_ptr<Sprite> spr);

    protected:
        std::forward_list<std::shared_ptr<Sprite>> _sprites;
        SweptAABBCollision _sac;

    };

}

