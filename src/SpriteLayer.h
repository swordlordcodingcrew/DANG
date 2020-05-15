// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Layer.h"

namespace dang
{
    using spSprite = std::shared_ptr<Sprite>;

    class SpriteLayer : public Layer
    {
    public:
        SpriteLayer();
        explicit SpriteLayer(const RectF& layer_size_px);
        ~SpriteLayer() override = default;

        void    update(uint32_t time, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual void    addSprite(spSprite spr);
        virtual void    removeSprite(spSprite spr);

    protected:
        explicit SpriteLayer(Layer::E_TYPE type) : Layer(type) {};
        SpriteLayer(Layer::E_TYPE type, const RectF& layer_size_px) : Layer(type, layer_size_px) {};

        std::forward_list<spSprite> _sprites;

    };

}

