// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "SprLayer.hpp"
#include "Gear.hpp"
#include "Layer.hpp"
#include "SprIterator.hpp"
#include "sprite/ImgSpr.hpp"

#include <cassert>
#include <memory>

namespace dang
{
    SprLayer::SprLayer() : Layer(Layer::LT_SPRITELAYER)
    {
        _root = std::make_shared<ImgSpr>();
    }

    spImgSpr SprLayer::getSpriteByTypeNum(uint8_t type_num)
    {
        for (SprIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->_type_num == type_num)
            {
                return std::dynamic_pointer_cast<ImgSpr>(*it);
            }
        }

        return nullptr;
    }


    spImgSpr SprLayer::getSpriteById(uint16_t id)
    {
        for (SprIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->_id == id)
            {
                return std::dynamic_pointer_cast<ImgSpr>(*it);
            }
        }

        return nullptr;
    }

    void SprLayer::markRemoveSpritesByTypeNum(uint8_t type_num)
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_type_num == type_num)
            {
                (*sti)->markRemove();
            }
            ++sti;
        }
    }

    void SprLayer::addSprite(spImgSpr s)
    {
        assert(s != nullptr);
        _root->addSpriteObject(s);
    }

    void SprLayer::_removeSprite(spImgSpr s)
    {
        assert(s != nullptr);
        s->removeMeFromTree();
    }

    void SprLayer::update(uint32_t dt, const Gear &gear)
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
    void SprLayer::coreUpdate(uint32_t dt, const Gear &gear)
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
                if ((*sti)->_parent == nullptr)
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

    void SprLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();
        int32_t vpx = std::floor(vp.tl().x);
        int32_t vpy = std::floor(vp.tl().y);

        for (SprIterator it = begin(); it != end(); it++)
        {
            spImgSpr s = std::dynamic_pointer_cast<ImgSpr>(*it);
            s->render(vpx, vpy);
        }
    }

    SprIterator SprLayer::begin()
    {
        return SprIterator(_root, _root);
    }

    SprIterator SprLayer::end()
    {
        return SprIterator(nullptr, _root);
    }

    SprIterator SprLayer::erase(SprIterator pos)
    {
        spSprObj s = (*pos);
        pos++;

        s->removeMeFromTree();

        return pos;
    }

    void SprLayer::cleanSpritelist()
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
                sti++;
            }
        }
    }
}
