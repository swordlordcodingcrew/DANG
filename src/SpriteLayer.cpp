// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <32blit.hpp>
#include <cassert>
#include "Gear.hpp"
#include "SpriteLayer.hpp"
#include "Sprite.hpp"
#include "Layer.hpp"
#include "Imagesheet.hpp"
#include "SpriteIterator.hpp"

namespace dang
{
    dang::SpriteLayer::SpriteLayer() : Layer(Layer::LT_SPRITELAYER)
    {
        _root = std::make_shared<Sprite>();
        _root->_visible = false;
        _root->setPos({0,0});
    }

    SpriteLayer::SpriteLayer(const tmx_layer *l) : Layer(Layer::LT_SPRITELAYER, l)
    {
        _root = std::make_shared<Sprite>();
        _root->_visible = false;
        _root->setPos({0,0});

    }

    spSprite SpriteLayer::getSpriteByType(const std::string& name)
    {
        spSprite ret{nullptr};
        for (SpriteIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->_type_name == name)
            {
                ret = (*it);
                break;
            }
        }

        return ret;
    }


    spSprite SpriteLayer::getSpriteById(uint16_t id)
    {
        spSprite ret{nullptr};
        for (SpriteIterator it = begin(); it != end(); ++it)
        {
            if ((*it)->_id == id)
            {
                ret = (*it);
                break;
            }
        }

        return ret;
    }

    void SpriteLayer::markRemoveSpritesByTypeNum(uint8_t type_num)
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_type_num == type_num)
            {
                (*sti)->markRemove();
//                sti = erase(sti);
            }
            ++sti;

        }
    }

    void SpriteLayer::addSprite(spSprite s)
    {
        assert(s != nullptr);
        _root->addSprite(s);
    }

    void SpriteLayer::_removeSprite(spSprite s)
    {
        assert(s != nullptr);
        s->removeMeFromTree();
    }

    void SpriteLayer::update(uint32_t dt, const Gear &gear)
    {
        // core update and update active sprites
        coreUpdate(dt, gear);

        // then call update
        for (SpriteIterator it = begin(); it != end(); it++)
        {
            if ((*it)->_active)
            {
                (*it)->update(dt);
            }
        }
    }

    void SpriteLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        // update active sprites (tree) or set to inactive if outside active world
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
            {
                sti = erase(sti);
            }
            else
            {
//                if ((*sti)->_parent.expired())
                if ((*sti)->_parent == nullptr)
                {
                    // root element. Always in
                    (*sti)->coreUpdate(dt);
                }
                else
                {
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRectG());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->_active = true;
                    }
                    else if ((*sti)->_active)
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->_active = false;
                    }
                }
                sti++;
            }
        }
    }

    void SpriteLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();
        int32_t vpx = std::floor(vp.tl().x);
        int32_t vpy = std::floor(vp.tl().y);

        for (SpriteIterator it = begin(); it != end(); it++)
        {
            if ((*it)->_visible && (*it)->_active && (*it)->_imagesheet != nullptr && vp.intersects((*it)->getSizeRectG()))
            {
                (*it)->render(vpx, vpy);
            }
        }
    }

    SpriteIterator SpriteLayer::begin()
    {
        return SpriteIterator(_root, _root);
    }

    SpriteIterator SpriteLayer::end()
    {
        return SpriteIterator(nullptr, _root);
    }

    SpriteIterator SpriteLayer::erase(SpriteIterator pos)
    {
        spSprite s = (*pos);
        pos++;

        s->removeMeFromTree();

        return pos;
    }

    void SpriteLayer::cleanSpritelist()
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
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
