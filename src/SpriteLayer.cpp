// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 02.03.20.
//

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

    SpriteLayer::SpriteLayer(const RectF& layer_size_px) : Layer(Layer::LT_SPRITELAYER, layer_size_px)
    {

    }

    SpriteLayer::~SpriteLayer()
    {

    }

    void SpriteLayer::update(uint32_t time, const Gear &gear)
    {
       for (spSprite& spr : _sprites)
       {
           RectF dr = gear.getActiveWorld().intersection(spr->getSizeRect());
           if (dr.area() != 0)
           {
               spr->coreUpdate(time);

               spr->update(time);
           }
       }

       _sac.handleCollisionDetection(_sprites);

    }

    void SpriteLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();

        for (std::shared_ptr<Sprite>& spr : _sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr)
            {
                RectF dr = vp.intersection(spr->getSizeRect());
                if (dr.area() != 0)
                {
                    if (blit::screen.sprites != spr->_imagesheet.get())
                    {
                        blit::screen.sprites = spr->_imagesheet.get();
                    }

                    blit::Rect sr = spr->_imagesheet->getRect(spr->_img_index);
                    Vector2F dp = spr->getPos() - vp.tl();
                    blit::screen.blit_sprite(sr, blit::Point(int32_t(std::floor(dp.x)), int32_t(std::floor(dp.y))), spr->_transform);

                }

            }
#ifdef __GEAR_DEBUG

            if(!spr->_visible)
            {
                blit::screen.pen(blit::RGBA(0, 255, 0, 255));

            }
            else
            {
                blit::screen.pen(blit::RGBA(0, 0, 255, 255));
            }

            RectF ddr = gear.getViewport().intersection(spr->getSizeRect());
            if (ddr.area() != 0)
            {
                ddr.x -= gear.getViewport().x;
                ddr.y -= gear.getViewport().y;

                blit::Point tl(int32_t(ddr.tl().x), int32_t(ddr.tl().y));
                blit::Point bl(int32_t(ddr.bl().x), int32_t(ddr.bl().y));
                blit::Point br(int32_t(ddr.br().x), int32_t(ddr.br().y));
                blit::Point tr(int32_t(ddr.tr().x), int32_t(ddr.tr().y));

                blit::screen.line(tl, bl); // left -> bottom
                blit::screen.line(bl, br); // bottom -> right
                blit::screen.line(br, tr); // right -> top
                blit::screen.line(tr, tl); // top -> left
            }
            blit::screen.pen(blit::RGBA(0, 0, 0, 255));
#endif

        }

    }


    void SpriteLayer::addSprite(std::shared_ptr<Sprite> spr)
    {
        _sprites.push_front(spr);
        _sprites.sort([] (const std::shared_ptr<Sprite> &first, const std::shared_ptr<Sprite> &second)
          {
              return first->_z_order < second->_z_order;
          });
    }

    void SpriteLayer::removeSprite(std::shared_ptr<Sprite> spr)
    {
        _sprites.remove(spr);
    }


}
