// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <32blit.hpp>
#include "BaseHUDLayer.hpp"
#include "CollisionSprite.hpp"
#include "Sprite.hpp"
#include "Gear.hpp"
#include "Imagesheet.hpp"

namespace dang
{
    BaseHUDLayer::BaseHUDLayer() : Layer(LT_HUDLAYER)
    {

    }

    void BaseHUDLayer::update(uint32_t dt, const Gear &gear)
    {
        // call update, updating all sprites, no filter
        for (spSprite& spr : _sprites)
        {
            spr->update(dt);
        }

        updateInternal(dt, gear);
    }

    void BaseHUDLayer::render(const Gear &gear)
    {
        RectF vp = {0, 0, float(blit::screen.bounds.w), float(blit::screen.bounds.h)};
//        RectF vp = {0, 0, gear.getViewport().w, gear.getViewport().h};

        for (std::shared_ptr<Sprite>& spr : _sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr)
            {
                RectF dr = vp.intersection(spr->getSizeRect());
                if (dr.area() != 0)
                {
                    Vector2F vec = spr->getPos() - vp.tl();
                    blit::Point dp = {int32_t(std::floor(vec.x)), int32_t(std::floor(vec.y))};

                    blit::screen.blit(spr->_imagesheet->getSurface(), spr->getBlitRect(), dp, spr->_transform);
                }
            }
        }

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


    void BaseHUDLayer::addSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _sprites.push_front(spr);
        }
    }

    void BaseHUDLayer::removeSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _sprites.remove(spr);
        }
    }

    spSprite BaseHUDLayer::getSpriteById(uint16_t id)
    {

        auto ret = std::find_if(_sprites.begin(), _sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
        {
            return first->_id == id;
        });

        if (ret != _sprites.end())
        {
            return (*ret);
        }

        return nullptr;
    }

    void BaseHUDLayer::removeSpriteById(uint16_t id)
    {
        _sprites.remove_if([&] (const std::shared_ptr<Sprite> &spr)
                                  {
                                      return spr->_id == id;
                                  });
    }

}