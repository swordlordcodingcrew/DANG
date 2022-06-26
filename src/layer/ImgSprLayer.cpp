// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ImgSprLayer.hpp"
#include "Gear.hpp"
#include "SprIterator.hpp"
#include "sprite/ImgSpr.hpp"

#include <cassert>
#include <memory>

namespace dang
{
    ImgSprLayer::ImgSprLayer() : SprLayer(Layer::LT_SPRITELAYER)
    {
        spImgSpr r = std::make_shared<ImgSpr>();
        r->setVisible(false);
        setRoot(r);
    }

    ImgSprLayer::ImgSprLayer(const tmx_layer *l) : SprLayer(Layer::LT_SPRITELAYER, l)
    {
        spImgSpr r = std::make_shared<ImgSpr>();
        r->setVisible(false);
        setRoot(r);
    }

    void ImgSprLayer::addSprite(spImgSpr s)
    {
        assert(s != nullptr);
        assert(s->getImagesheet() != nullptr);
        getRoot()->addSpriteObject(s);
    }

    void ImgSprLayer::update(uint32_t dt, const Gear &gear)
    {
        // core update and update active sprites
        coreUpdate(dt, gear);

        // then call update
        for (SprIterator it = begin(); it != end(); it++)
        {
            if ((*it)->isActive() && (*it)->inZone())
            {
                (*it)->update(dt);
            }
        }
    }

    /**
     * internal update of layer and sprites.
     * - remove sprite which are marked to be removed
     * - check inside/outside active zone
     * - if active set to false, skip spr
     * @param dt
     * @param gear
     */
    void ImgSprLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->isMarkedRemove())
            {
                sti = erase(sti);
            }
            else
            {
                if (isRoot(*sti))
                {
                    // root element. Always in
                    (*sti)->coreUpdate(dt);
                }
                else if ((*sti)->isActive())
                {
                    spImgSpr imgspr = std::dynamic_pointer_cast<ImgSpr>(*sti);
                    bool in_active_world = gear.getActiveWorld().intersects(imgspr->getSizeRectG());
                    if (in_active_world)
                    {
                        imgspr->coreUpdate(dt);
                        imgspr->setInZone(true);
                    }
                    else if (imgspr->inZone())
                    {
                        imgspr->coreUpdate(dt);
                        imgspr->setInZone(false);
                    }
                }
                sti++;
            }
        }
    }

    void ImgSprLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();
        int32_t vpx = std::floor(vp.tl().x);
        int32_t vpy = std::floor(vp.tl().y);

        for (SprIterator it = begin(); it != end(); it++)
        {
            spImgSpr s = std::dynamic_pointer_cast<ImgSpr>(*it);
            // check if visible, active, inzone is done in the sprite
            s->render(vpx, vpy);
        }
    }

}
