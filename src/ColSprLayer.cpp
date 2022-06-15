// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ColSprLayer.hpp"
#include "SpriteIterator.hpp"
#include "Gear.hpp"

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
/*    CollisionSpriteLayer::CollisionSpriteLayer()
    {
        _type = LT_COLLISIONSPRITELAYER;
        _root = std::make_shared<CollisionSprite>();
        _root->_visible = false;
        _root->setPos({0,0});
    }

    void CollisionSpriteLayer::addCollisionSprite(spCollisionSprite cspr)
    {
        SpriteLayer::addSprite(cspr);
        cspr->setCSPosition(cspr->local2Global(cspr->getPos()));
        if (cspr->_active)
        {
            _cs.addCObject(cspr);
        }
    }

    void CollisionSpriteLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        // update active sprites (tree) or set to inactive if outside active zone
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
            {
                sti = erase(sti);
            }
            else
            {
                if ((*sti)->_parent == nullptr)
                {
                    // root element. Always in
                    // but not in collision solver
                    (*sti)->coreUpdate(dt);
                }
                else
                {
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRectG());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        if (!(*sti)->_active)
                        {
                            (*sti)->_active = true;
                            spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                            co->setCSPosition((*sti)->local2Global((*sti)->getPos()));
                            _cs.addCObject(co);
                        }
                    }
                    else if ((*sti)->_active)
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->_active = false;
                        spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                        co->remove();
                    }
                }
                sti++;
            }
        }
    }

    void CollisionSpriteLayer::update(uint32_t dt, const Gear &gear)
    {
        coreUpdate(dt, gear);

        // collision resolution
        _cs.solve();

        // then call update
        for (SpriteIterator it = begin(); it != end(); it++)
        {
            spCollisionSprite cspr = std::dynamic_pointer_cast<CollisionSprite>((*it));
            if (cspr->_active)
            {
                cspr->update(dt);
            }
        }


    }

    void CollisionSpriteLayer::render(const Gear &gear)
    {
        SpriteLayer::render(gear);

#ifdef DANG_DEBUG_DRAW

        #ifdef TARGET_32BLIT_HW
        // show memory stats
        auto static_used = &_end - &_sbss;
        auto heap_total = &__ltdc_start - &_end;
        auto heap_used = mallinfo().uordblks;

        auto total_ram = static_used + heap_total;

        blit::screen.pen = blit::Pen(255, 0, 255, 255);
        char buf[100];
        snprintf(buf, sizeof(buf), "Mem: %i + %i / %i", static_used, heap_used, total_ram);
        blit::screen.text(buf, hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);
    #else

        // show amount of sprites and memory used
/*        std::stringstream stats;

        stats << "active: " << _active_sprites.size() << " inactive: " << _inactive_sprites.size() << " heap: " << mallinfo().uordblks;
        blit::screen.text(stats.str(), hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);

        if (_dbg_mem < mallinfo().uordblks)
        {
            _dbg_mem = mallinfo().uordblks;
            std::cout << "CSL.render() " << stats.str() << std::endl;
        }
*/
/*        // show hotrects
        RectF vp = gear.getViewport();

        blit::screen.pen = blit::Pen(0, 0, 255, 255);
        for (SpriteIterator it = begin(); it != end(); it++)
//        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            spCollisionSprite cspr = std::static_pointer_cast<CollisionSprite>(*it);
            RectF dr = vp.intersection(cspr->getSizeRect());

            if (dr.area() != 0)
            {
                RectF hr = cspr->getHotrectG();
                hr.x -= vp.tl().x;
                hr.y -= vp.tl().y;

                blit::Point tl(int32_t(hr.tl().x), int32_t(hr.tl().y));
                blit::Point bl(int32_t(hr.bl().x), int32_t(hr.bl().y));
                blit::Point br(int32_t(hr.br().x), int32_t(hr.br().y));
                blit::Point tr(int32_t(hr.tr().x), int32_t(hr.tr().y));

                blit::screen.line(tl, bl); // left -> bottom
                blit::screen.line(bl, br); // bottom -> right
                blit::screen.line(br, tr); // right -> top
                blit::screen.line(tr, tl); // top -> left
            }
        }
        #endif
#endif
    }

    void CollisionSpriteLayer::cleanSpritelist()
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
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

    void CollisionSpriteLayer::_removeSprite(spSprite s)
    {
        assert(s != nullptr);
        s->removeMeFromTree();
        spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>(s);
        _cs.removeCObject(co);
    }

    float CollisionSpriteLayer::aaLoSH(const CollisionSprite& me, const CollisionSprite& target)
    {
        return _cs.aaLoSH(static_cast<const CollisionObject*>(&me), static_cast<const CollisionObject*>(&target));
    }

    float CollisionSpriteLayer::loS(const CollisionSprite& me, const CollisionSprite& target)
    {
        return _cs.loS(static_cast<const CollisionObject*>(&me), static_cast<const CollisionObject*>(&target));
    }
*/
}