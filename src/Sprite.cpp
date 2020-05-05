// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include "Sprite.h"
#include "tmx_def.h"
#include "tween/Tweenable.h"
#include "SweptAABBCollision.h"

namespace dang
{
    Sprite::Sprite()
    {
    }

    Sprite::~Sprite()
    {

    }

/*    Sprite::Sprite(const Sprite &sp)
    {

    }
*/
    Sprite::Sprite(const tmx_spriteobject &so, std::shared_ptr<Imagesheet> is)
    {
        _id = so.id; // global
        _type = so.type;
        _pos.x = so.x;
        _pos.y = so.y;
        _size.w = so.width;
        _size.h = so.height;
        _visible = so.visible;
        _img_index = so.tile;
        _imagesheet = is;
        _hotrect = {0, 0, float(so.width), float(so.height)};
        _last_pos = _pos;
    }

/*    uint16_t Sprite::collisionResponse(std::shared_ptr<Sprite> other)
    {
        if (_type == "hotrect" && other->_type == "hotrect") return SweptAABBCollision::CR_NONE;

/*        if (_type == "coin")
        {
            return cross_me;
        }
        else if (other->_type == "hero")
        {
            return slide_over_me;
        }
        else if(_type == "hero")
        {
            return touch_me;
        }
*//*
        return SweptAABBCollision::CR_SLIDE;
    }
*/
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

    void Sprite::coreUpdate(uint32_t time)
    {
        _last_pos = _pos;
        _is_hit = false;
        updateTweens(time);
    }

    void Sprite::update(uint32_t time)
    {
        // dt in ?
        float dt = float(time - (_last_update_time == 0 ? time : _last_update_time)) / 100;
        _last_update_time = time;
        _vel += _acc * dt;
        _pos += _vel * dt;
    }

    RectF Sprite::getSizeRect()
    {
        return RectF(_pos.x, _pos.y, _size.x, _size.y);
    }

    RectF Sprite::getHotrectAbs()
    {
        return RectF(_hotrect.x + _pos.x, _hotrect.y + _pos.y, _hotrect.w, _hotrect.h);
    }

    void Sprite::collide(const manifold &mf)
    {
        switch (_coll_response)
        {
            case SweptAABBCollision::CR_BOUNCE:
                if (mf.normalMe.x != 0)
                {
                    _vel.x = -_vel.x;
                }
                else
                {
                    _vel.y = -_vel.y;
                }
                break;
            case SweptAABBCollision::CR_SLIDE:
                if (mf.normalMe.x != 0)
                {
                    _vel.y = 0;
                }
                else
                {
                    _vel.x = 0;
                }
                break;
            case SweptAABBCollision::CR_TOUCH:
                _vel = {0,0};
                break;
        }
    }

}
