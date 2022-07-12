// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ColSprLayer.hpp"
#include "Gear.hpp"
#include "sprite/ColSpr.hpp"
#include "SprIterator.hpp"

#include <iostream>
#include <sstream>
#include <malloc.h>
#include <cassert>

#ifdef DANG_DEBUG_DRAW
#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#include <malloc.h>
#include "../../../fonts/hud_font_small.h"
extern char _sbss, _end, __ltdc_start;
#endif
#endif

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
        if (cspr->isActive())
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
//                    spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRectG());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        if (!(*sti)->inZone())
                        {
                            (*sti)->setInZone(true);
                            spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                            co->setCSPosition((*sti)->local2Global((*sti)->getPos()));
                            _cs.addCObject(co);
                        }

                    }
                    else if ((*sti)->inZone())
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->setInZone(false);
                        spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
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
                spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>(*sti);
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

}