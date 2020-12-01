// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "Gear.h"
#include "SpriteLayer.h"
#include "Sprite.h"
#include "Layer.h"
#include "Imagesheet.h"

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
                    gear.set_spritesheet_cb(spr->_imagesheet);
/*                    if (blit::screen.sprites != spr->_imagesheet.get())
                    {
                        blit::screen.sprites = spr->_imagesheet.get();
                    }
*/
                    RectU sr = spr->_imagesheet->getRect(spr->_img_index);
                    Vector2F vec = spr->getPos() - vp.tl();
                    Vector2I dp = {int32_t(std::floor(vec.x)), int32_t(std::floor(vec.y))};
                    gear.blit_sprite_cb(sr, dp, spr->_transform);
//                    blit::screen.blit_sprite(sr, blit::Point(int32_t(std::floor(dp.x)), int32_t(std::floor(dp.y))), spr->_transform);
                }
            }

#ifdef DANG_DEBUG_DRAW
            if(!spr->_visible)
            {
                blit::screen.pen = blit::Pen(0, 255, 0, 255);

            }
            else
            {
                blit::screen.pen = blit::Pen(0, 0, 255, 255);
            }

            RectF dr = vp.intersection(spr->getSizeRect());
//            RectF dr = vp.intersection(spr->getSizeRect());
            if (dr.area() != 0)
            {
                dr.x -= vp.x;
                dr.y -= vp.y;

                blit::Point tl(int32_t(dr.tl().x), int32_t(dr.tl().y));
                blit::Point bl(int32_t(dr.bl().x), int32_t(dr.bl().y));
                blit::Point br(int32_t(dr.br().x), int32_t(dr.br().y));
                blit::Point tr(int32_t(dr.tr().x), int32_t(dr.tr().y));

                blit::screen.line(tl, bl); // left -> bottom
                blit::screen.line(bl, br); // bottom -> right
                blit::screen.line(br, tr); // right -> top
                blit::screen.line(tr, tl); // top -> left
            }
//            blit::screen.pen = blit::Pen(0, 0, 0, 255);
#endif

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

}
