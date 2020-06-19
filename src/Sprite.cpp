// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <algorithm>
#include "Sprite.h"
#include "TmxExtruder.h"
#include "tween/Tweenable.h"
#include "tween/TwAnim.h"
#include "CollisionSpriteLayer.h"

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
        _last_pos = _pos;
    }

    void Sprite::addTween(std::shared_ptr<Tweenable> tw)
    {
        // this is a weak ptr reference to omit a dangling pointer (or circular reference)
        tw->setObject(shared_from_this());
        _tweens.push_front(tw);
    }

    void Sprite::updateTweens(uint32_t time)
    {
        std::list<std::shared_ptr<Tweenable>>::iterator tw = _tweens.begin();
        while (tw != _tweens.end())
        {
            if (!(*tw))
            {
                // somehow a stale tween
                tw = _tweens.erase(tw);
            }
            else
            {
                (*tw)->update(time);
                if ((*tw)->isTwFinished())
                {
                    tw = _tweens.erase(tw);
                }
                else
                {
                    ++tw;
                }
            }

        }

        // special case animation. There is only one
        if (_animation)
        {
            _animation->update(time);
            if (_animation->isTwFinished())
            {
                _animation.reset();
            }
        }
    }

    void Sprite::removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB)
    {
        tw->finishTw(suppressCB);
        _tweens.remove(tw);
        tw->clearObject();
    }

    void Sprite::coreUpdate(uint32_t time)
    {
        _last_pos = _pos;
        updateTweens(time);
    }

    void Sprite::update(uint32_t time)
    {
        // dt in 10 ms
        float dt = float(time - (_last_update_time == 0 ? time : _last_update_time)) / 100;
        _last_update_time = time;
        _vel += _acc * dt;
        _pos += _vel * dt;
    }

    RectF Sprite::getSizeRect()
    {
        return RectF(_pos.x, _pos.y, _size.x, _size.y);
    }

    bool Sprite::tweenActive(const std::shared_ptr<Tweenable> &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }

    void Sprite::removeAnimation(bool suppressCB)
    {
        if (_animation)
        {
            _animation->finishTw(suppressCB);
            _animation->clearObject();
            _animation.reset();
        }
    }

    void Sprite::setAnimation(std::shared_ptr<TwAnim> twa)
    {
        _animation = twa;
        _animation->setObject(shared_from_this());
    }

}
