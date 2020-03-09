// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 02.03.20.
//

#include "Gear.h"
#include "SpriteLayer.h"
#include "Sprite.h"
#include "Layer.h"

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
        for (std::shared_ptr<Sprite>& spr : _sprites)
        {
            if (spr->_visible)
            {
                blit::Rect dr = gear.getViewport().intersection(spr->getSizeRect());
                if (!dr.empty())
                {
                    gear.renderImage(spr->_imagesheet, spr->_img_index, spr->getPos(), spr->_transform);
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

            blit::Rect dr = gear.getViewport().intersection(spr->getSizeRect());
            blit::screen.line(dr.tl(), dr.bl()); // left -> bottom
            blit::screen.line(dr.bl(), dr.br()); // bottom -> right
            blit::screen.line(dr.br(), dr.tr()); // right -> top
            blit::screen.line(dr.tr(), dr.tl()); // top -> left

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
