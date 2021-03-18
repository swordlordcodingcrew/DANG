// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <list>

#include "Layer.hpp"
#include "SpriteLayer.hpp"

namespace dang
{
    using spSprite = std::shared_ptr<Sprite>;

    class BaseHUDLayer : public SpriteLayer
    {
    public:
        BaseHUDLayer();
        ~BaseHUDLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

    protected:
        virtual void updateInternal(uint32_t dt, const Gear& gear) = 0;
        virtual void renderInternal(const Gear& gear) = 0;
    };
}

