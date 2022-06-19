// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <list>

#include "Layer.hpp"

namespace dang
{
    class CollisionSprite;

    using spSprite = std::shared_ptr<Sprite>;
    using spCollisionSprite = std::shared_ptr<CollisionSprite>;

    class BaseHUDLayer : public Layer
    {
    public:
        BaseHUDLayer();
        BaseHUDLayer(const tmx_layer* l);
        ~BaseHUDLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual void    addSprite(spSprite spr);
        virtual void    removeSprite(spSprite spr);
        virtual void    removeSpriteById(uint16_t id);
        virtual spSprite    getSpriteById(uint16_t id);


    protected:
        virtual void updateInternal(uint32_t dt, const Gear& gear) = 0;
        virtual void renderInternal(const Gear& gear) = 0;

        // all sprites, no active vs inactive as SpriteLayer knows
        std::list<spSprite> _sprites;
    };
}

