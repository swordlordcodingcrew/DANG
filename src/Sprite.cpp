// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 31.1.20.
//

#include "Sprite.h"

namespace dang
{
    Sprite::Sprite()
    {

    }

    Sprite::~Sprite()
    {

    }

    Sprite::Sprite(const Sprite &sp)
    {

    }

    Sprite::Sprite(const spriteobject &so)
    {
        _id = so.id; // global
        _type = so.type;
        _pos.x = so.x;
        _pos.y = so.y;
        _size.w = so.width;
        _size.h = so.height;
        _visible = so.visible;
        _img_index = so.tile;
        //_tileset = so.tileset;
    }


    uint16_t Sprite::wantToCollideWith(std::shared_ptr<sprite> other)
    {
        if (_type == "coin")
        {
            return cross_me;
        }
        else if (other->type == "hero")
        {
            return slide_over_me;
        }
        else if(_type == "hero")
        {
            return touch_me;
        }

        return 0;
    }

    void Sprite::addTween(std::shared_ptr<Tweenable> tw)
    {
        _tweens.push_front(tw);
    }

    void Sprite::updateTweens(uint32_t time)
    {
        std::list<std::shared_ptr<Tweenable>>::iterator tw = _tweens.begin();
        while (tw != _tweens.end())
        {
            (*tw)->update(time);
            if ((*tw)->is_tw_finished())
            {
                tw = _tweens.erase(tw);
            }
            else
            {
                ++tw;
            }
        }
    }

    void Sprite::removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB)
    {
        tw->finish_tw(suppressCB);
        _tweens.remove(tw);

    }

}
