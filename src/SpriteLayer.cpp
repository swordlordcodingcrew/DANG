// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <32blit.hpp>
#include "Gear.hpp"
#include "SpriteLayer.hpp"
#include "Sprite.hpp"
#include "Layer.hpp"
#include "Imagesheet.hpp"

namespace dang
{
    dang::SpriteLayer::SpriteLayer() : Layer(Layer::LT_SPRITELAYER)
    {

    }

    void SpriteLayer::update(uint32_t dt, const Gear &gear)
    {
        // core update and update active sprites
        coreUpdate(dt, gear);

        // then call update
        for (spSprite& spr : _active_sprites)
        {
            if (gear.getActiveWorld().intersects(spr->getSizeRect()))
            {
                spr->update(dt);
            }
        }
    }

    void SpriteLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();

        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr)
            {
                RectF dr = vp.intersection(spr->getSizeRect());
                if (dr.area() != 0)
                {
                    if (blit::screen.sprites != spr->_imagesheet->getSurface())
                    {
                        blit::screen.sprites = spr->_imagesheet->getSurface();
                    }
                    blit::Point dp;
                    dp.x  = int32_t(std::floor(spr->getPos().x) - std::floor(vp.tl().x));
                    dp.y  = int32_t(std::floor(spr->getPos().y) - std::floor(vp.tl().y));

//                    blit::Point dp = {int32_t(std::floor(vec.x)), int32_t(std::floor(vec.y))};

                    blit::screen.blit_sprite(spr->getBlitRect(), dp, spr->_transform);
                }
            }
        }
    }


    void SpriteLayer::addSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _inactive_sprites.push_front(spr);
        }
    }

    void SpriteLayer::removeSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _active_sprites.remove(spr);
            _inactive_sprites.remove(spr);
        }
    }

    void SpriteLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        std::list<spSprite> splice_list;

        // find inactive sprites becoming active, call coreUpdate and put them into the splice_list
        auto spr = _inactive_sprites.begin();
        while (spr != _inactive_sprites.end())
        {
            if (gear.getActiveWorld().intersects((*spr)->getSizeRect()))
            {
                splice_list.push_front(*spr);
                (*spr)->coreUpdate(dt);
                spr = _inactive_sprites.erase(spr);
            }
            else
            {
                spr++;
            }
        }

        // update active sprite or put them into the inactive sprite list
        spr = _active_sprites.begin();
        while (spr != _active_sprites.end())
        {
            if (gear.getActiveWorld().intersects((*spr)->getSizeRect()))
            {
                (*spr)->coreUpdate(dt);
                spr++;
            }
            else
            {
                _inactive_sprites.push_front(*spr);
                spr = _active_sprites.erase(spr);
            }
        }

        if (!splice_list.empty())
        {
#ifdef DANG_DEBUG
            std::cout << "merge active:" << splice_list.size() << std::endl;
#endif
            _active_sprites.merge(splice_list, [] (const std::shared_ptr<Sprite> &first, const std::shared_ptr<Sprite> &second)
            {
                return first->_z_order < second->_z_order;
            });
        }

    }

    spSprite SpriteLayer::getSpriteById(uint16_t id)
    {

        auto ret = std::find_if(_active_sprites.begin(), _active_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
            {
                return first->_id == id;
            });

        if (ret != _active_sprites.end())
        {
            return (*ret);
        }

        ret = std::find_if(_inactive_sprites.begin(), _inactive_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
            {
                return first->_id == id;
            });

        if (ret != _active_sprites.end())
        {
            return (*ret);
        }

        return nullptr;
    }

    void SpriteLayer::removeSpriteById(uint16_t id)
    {
        _active_sprites.remove_if([&] (const std::shared_ptr<Sprite> &spr)
            {
              return spr->_id == id;
            });

        _inactive_sprites.remove_if([&] (const std::shared_ptr<Sprite> &spr)
            {
                return spr->_id == id;
            });
    }

}
