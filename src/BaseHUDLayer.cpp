// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "BaseHUDLayer.hpp"
#include "CollisionSprite.hpp"
#include "Sprite.hpp"
#include "Gear.hpp"

namespace dang
{
    BaseHUDLayer::BaseHUDLayer() : SpriteLayer(LT_HUDLAYER)
    {

    }

    void BaseHUDLayer::update(uint32_t dt, const Gear &gear)
    {
        coreUpdate(dt, gear);

        // then call update
        for (spSprite& spr : _active_sprites)
        {
            if (gear.getActiveWorld().intersects(spr->getSizeRect()))
            {
                spr->update(dt);
            }
        }

        updateInternal(dt, gear);
    }

    void BaseHUDLayer::render(const Gear &gear)
    {
        SpriteLayer::render(gear);

#ifdef DANG_DEBUG_DRAW

/*        RectF vp = gear.getViewport();

        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            RectF dr = vp.intersection(spr->getSizeRect());

            if (dr.area() != 0)
            {
                spCollisionSprite cspr = std::static_pointer_cast<CollisionSprite>(spr);
                RectF hr = cspr->getHotrectAbs();
                hr.x -= vp.tl().x;
                hr.y -= vp.tl().y;

                Vector2I tl(int32_t(hr.tl().x), int32_t(hr.tl().y));
                Vector2I bl(int32_t(hr.bl().x), int32_t(hr.bl().y));
                Vector2I br(int32_t(hr.br().x), int32_t(hr.br().y));
                Vector2I tr(int32_t(hr.tr().x), int32_t(hr.tr().y));

                gear.line_cb(tl, bl, 0, 0, 255, 255); // left -> bottom
                gear.line_cb(bl, br, 0, 0, 255, 255); // bottom -> right
                gear.line_cb(br, tr, 0, 0, 255, 255); // right -> top
                gear.line_cb(tr, tl, 0, 0, 255, 255); // top -> left
            }
        }
        */
#endif

        renderInternal(gear);
    }

}