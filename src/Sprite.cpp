// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <algorithm>
#include "Sprite.hpp"
#include "TmxExtruder.hpp"
#include "tween/Tweenable.hpp"
#include "tween/TwAnim.hpp"
#include "CollisionSpriteLayer.hpp"

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

    void Sprite::updateTweens(uint32_t dt)
    {
        std::list<std::shared_ptr<Tweenable>>::iterator tw = _tweens.begin();
        while (tw != _tweens.end())
        {
            if (!(*tw))
            {
                // somehow a stale tween
                (*tw)->clearObject();
                tw = _tweens.erase(tw);
            }
            else
            {
                (*tw)->update(dt);
                if ((*tw)->isFinished())
                {
                    (*tw)->clearObject();
                    tw = _tweens.erase(tw);
                }
                else
                {
                    ++tw;
                }
            }

        }

        // special case animation. There can be only one
        if (_animation)
        {
            _animation->update(dt);
            if (_animation->isFinished())
            {
                _animation->clearObject();
                _animation.reset();
            }
        }
    }

    void Sprite::removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB)
    {
        tw->finish(suppressCB);
        _tweens.remove(tw);
        tw->clearObject();
    }

    void Sprite::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            std::shared_ptr<Tweenable> tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool Sprite::tweenActive(const std::shared_ptr<Tweenable> &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }

    void Sprite::coreUpdate(uint32_t dt)
    {
        _last_pos = _pos;
        updateTweens(dt);

        // dt in 10 ms
        float dt10ms = dt / 100.0f;
        _vel += (_gravity + _acc) * dt10ms;
        _pos += _vel * dt10ms;
    }

    void Sprite::update(uint32_t dt)
    {
    }

    RectF Sprite::getSizeRect()
    {
        return RectF(_pos.x, _pos.y, _size.x, _size.y);
    }


    void Sprite::setAnimation(std::shared_ptr<Tweenable> twa)
    {
        _animation = twa;
        _animation->setObject(shared_from_this());
    }

    void Sprite::removeAnimation(bool suppressCB)
    {
        if (_animation)
        {
            _animation->finish(suppressCB);
            _animation->reset();
            _animation->clearObject();
            _animation.reset();
        }
    }

}
