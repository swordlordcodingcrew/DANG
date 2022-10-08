// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "BaseHUDLayer.hpp"
#include "Gear.hpp"
#include "Imagesheet.hpp"
#include "SprIterator.hpp"
#include "sprite/ImgSpr.hpp"

#include <iostream>

namespace dang
{
    BaseHUDLayer::BaseHUDLayer() : ImgSprLayer(LT_HUDLAYER)
    {
        spImgSpr r = std::make_shared<ImgSpr>();
        r->setVisible(false);
        setRoot(r);
    }

    BaseHUDLayer::BaseHUDLayer(const tmx_layer *l) : ImgSprLayer(LT_HUDLAYER, l)
    {
        spImgSpr r = std::make_shared<ImgSpr>();
        r->setVisible(false);
        setRoot(r);
    }


    void BaseHUDLayer::update(uint32_t dt, const Gear &gear)
    {
        // updating all sprites, no filter, no zones
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->isMarkedRemove())
            {
                sti = erase(sti);
            }
            else
            {
                (*sti)->coreUpdate(dt);
                sti++;
            }
        }

        sti = begin();
        while (sti != end())
        {
            (*sti)->update(dt);
            sti++;
        }

    }

    void BaseHUDLayer::render(const Gear &gear)
    {

        for (SprIterator it = begin(); it != end(); it++)
        {
            spImgSpr s = std::static_pointer_cast<ImgSpr>(*it);
            s->render(0, 0);
        }

/*
        RectF vp = {0, 0, float(blit::screen.bounds.w), float(blit::screen.bounds.h)};
        RectF vp = {0, 0, gear.getViewport().w, gear.getViewport().h};

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
*/
//        renderInternal(gear);
    }

/*
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

*/
}