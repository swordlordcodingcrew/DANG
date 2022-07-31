// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ColSprLayer.hpp"
#include "Gear.hpp"
#include "sprite/ColSpr.hpp"
#include "SprIterator.hpp"

namespace dang
{

    ColSprLayer::ColSprLayer() : SprLayer(Layer::LT_COLLISIONSPRITELAYER)
    {
        spColSpr r = std::make_shared<ColSpr>();
        setRoot(r);
    }

    ColSprLayer::ColSprLayer(const tmx_layer *l) : SprLayer(Layer::LT_COLLISIONSPRITELAYER, l)
    {
        spColSpr r = std::make_shared<ColSpr>();
        setRoot(r);
    }

    void ColSprLayer::addSprite(spColSpr cspr)
    {
        SprLayer::addSprite(cspr);
        cspr->setCSPosition(cspr->local2Global(cspr->getPos()));
        if (cspr->inZone())
        {
            _cs.addCObject(cspr);
        }
    }

    void ColSprLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        // update active sprites (tree) or set to inactive if outside active zone
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
                    // but not in collision solver
                    (*sti)->coreUpdate(dt);
                }
                else if ((*sti)->isActive())
                {
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRectG());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        if (!(*sti)->inZone())
                        {
                            (*sti)->setInZone(true);
                            spColSpr co = std::static_pointer_cast<ColSpr>(*sti);
//                            spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                            co->setCSPosition((*sti)->local2Global((*sti)->getPos()));
                            _cs.addCObject(co);
                        }

                    }
                    else if ((*sti)->inZone())
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->setInZone(false);
                        spColSpr co = std::static_pointer_cast<ColSpr>(*sti);
                        co->remove();
                    }
                }

                sti++;
            }
        }
    }

    void ColSprLayer::update(uint32_t dt, const Gear &gear)
    {
        coreUpdate(dt, gear);

        // collision resolution
        _cs.solve();

        // then call update
        for (SprIterator it = begin(); it != end(); it++)
        {
            spColSpr cspr = std::static_pointer_cast<ColSpr>((*it));
            if (cspr->isActive() && cspr->inZone())
            {
                cspr->update(dt);
            }
        }
    }

    void ColSprLayer::cleanSpritelist()
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->isMarkedRemove())
            {
                spColSpr cospr = std::static_pointer_cast<ColSpr>(*sti);
                spCollisionObject co = std::static_pointer_cast<CollisionObject>(cospr);
                _cs.removeCObject(co);
                sti = erase(sti);
            }
            else
            {
                sti++;
            }
        }
    }

    float ColSprLayer::aaLoSH(const ColSpr& me, const ColSpr& target)
    {
        return _cs.aaLoSH(static_cast<const CollisionObject*>(&me), static_cast<const CollisionObject*>(&target));
    }

    float ColSprLayer::loS(const ColSpr& me, const ColSpr& target)
    {
        return _cs.loS(static_cast<const CollisionObject*>(&me), static_cast<const CollisionObject*>(&target));
    }

    void ColSprLayer::render(const Gear &gear)
    {
        this->SprLayer::render(gear);
#ifdef DANG_DEBUG_COLSOLV
        blit::screen.pen = {0,0,0};
        // show number of objects in CollisionSolver
        blit::screen.text("#co: " + std::to_string(_cs.getListSize()), hud_font_small, { 50, 5 }, true, blit::TextAlign::top_left);
#endif
    }

    void ColSprLayer::resetZoneBit(const RectF &vp)
    {
        auto sti = begin();
        while (sti != end())
        {
            spColSpr spr = std::static_pointer_cast<ColSpr>(*sti);
            spCollisionObject co = std::static_pointer_cast<CollisionObject>(spr);
            _cs.removeCObject(co);
            bool inZone = vp.intersects((*sti)->getSizeRectG());
            (*sti)->setInZone(inZone);
            if (inZone)
            {
                _cs.addCObject(co);
            }
            sti++;
        }
    }

}