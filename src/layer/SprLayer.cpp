// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "SprLayer.hpp"
#include "Gear.hpp"
#include "src/layer/Layer.hpp"
#include "SprIterator.hpp"
#include "sprite/SpriteObject.hpp"

#include <cassert>
#include <memory>
#include <iostream>

namespace dang
{
    SprLayer::SprLayer() : Layer(Layer::LT_SPRITELAYER)
    {
    }

    SprLayer::SprLayer(const tmx_layer *l) : Layer(Layer::LT_SPRITELAYER, l)
    {
    }

    void SprLayer::setRoot(spSprObj root)
    {
        _root = root;
    }

    spSprObj SprLayer::getSpriteByTypeNum(uint8_t type_num)
    {
        for (SprIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->typeNum() == type_num)
            {
                return (*it);
            }
        }

        return nullptr;
    }


    spSprObj SprLayer::getSpriteById(uint16_t id)
    {
        for (SprIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->id() == id)
            {
                return (*it);
            }
        }

        return nullptr;
    }

    void SprLayer::markRemoveSpritesByTypeNum(uint8_t type_num)
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->typeNum() == type_num)
            {
                (*sti)->markRemove();
            }
            ++sti;
        }
    }

    void SprLayer::addSprite(spSprObj s)
    {
        assert(s != nullptr);
        _root->addSpriteObject(s);
    }

    void SprLayer::_removeSpriteFromTree(spSprObj s)
    {
        std::cout << "CAUTION: _removeSpriteFromTree in SprLayer called" << std::endl;
        assert(s != nullptr);
        s->removeMeFromTree();
    }

    void SprLayer::update(uint32_t dt, const Gear &gear)
    {
        // core update
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
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRect());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->setInZone(true);
                    }
                    else if ((*sti)->inZone())
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->setInZone(false);
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
            // check if visible, active, inzone is done in the sprite
            (*it)->render(vpx, vpy);
        }

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
#endif
#endif
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

    void SprLayer::resetZoneBit(const RectF& vp)
    {
        auto sti = begin();
        while (sti != end())
        {
            (*sti)->setInZone(vp.intersects((*sti)->getSizeRectG()));
            sti++;
        }

    }

    spSprObj &SprLayer::getRoot()
    {
        assert(_root != nullptr);
        return _root;
    }


}
