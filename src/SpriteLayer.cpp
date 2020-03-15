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

    SpriteLayer::SpriteLayer(const blit::Rect& layer_size_px) : Layer(Layer::LT_SPRITELAYER, layer_size_px)
    {

    }

    SpriteLayer::~SpriteLayer()
    {

    }

    void SpriteLayer::update(uint32_t time, const Gear &gear)
    {
       for (std::shared_ptr<Sprite>& spr : _sprites)
       {
           blit::Rect dr = gear.getActiveWorld().intersection(spr->getSizeRect());
           if (!dr.empty())
           {
               spr->coreUpdate(time);
           }

       }

       for (std::shared_ptr<Sprite>& spr : _sprites)
       {
           blit::Rect dr = gear.getActiveWorld().intersection(spr->getSizeRect());
           if (!dr.empty())
           {
               spr->update(time);
           }
       }

//       processCollisionDetection();

    }

    void SpriteLayer::render(const Gear &gear)
    {
        blit::Rect vp = gear.getViewport();

        for (std::shared_ptr<Sprite>& spr : _sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr)
            {
                blit::Rect dr = vp.intersection(spr->getSizeRect());
                if (!dr.empty())
                {
                    if (blit::screen.sprites != spr->_imagesheet.get())
                    {
                        blit::screen.sprites = spr->_imagesheet.get();
                    }

                    blit::Rect sr = spr->_imagesheet->getRect(spr->_img_index);
                    blit::screen.blit_sprite(sr, spr->getPos() - vp.tl(), spr->_transform);

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

            blit::Rect ddr = gear.getViewport().intersection(spr->getSizeRect());
            if (!ddr.empty())
            {
                ddr.x -= gear.getViewport().x;
                ddr.y -= gear.getViewport().y;
                blit::screen.line(ddr.tl(), ddr.bl()); // left -> bottom
                blit::screen.line(ddr.bl(), ddr.br()); // bottom -> right
                blit::screen.line(ddr.br(), ddr.tr()); // right -> top
                blit::screen.line(ddr.tr(), ddr.tl()); // top -> left
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
