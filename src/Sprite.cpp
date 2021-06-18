// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Sprite.hpp"
#include "TmxExtruder.hpp"
#include "tween/Tweenable.hpp"
#include "Imagesheet.hpp"

#include <32blit.hpp>

#include <algorithm>
#include <src/bt/bt.hpp>

#include <iostream>

namespace dang
{
    Sprite::Sprite()
    {
    }

    Sprite::~Sprite()
    {
        removeAnimation(true);
        removeTweens(true);
    }

    Sprite::Sprite(const Sprite &sp)
    : std::enable_shared_from_this<Sprite>(), _visible(sp._visible), _img_index(sp._img_index), _imagesheet(sp._imagesheet), _transform(sp._transform),
      _z_order(sp._z_order), _type_name(sp._type_name), _type_num(sp._type_num), _size(sp._size), _pos(sp._pos), _vel(sp._vel),
      _acc(sp._acc), _gravity(sp._gravity), _last_pos(sp._last_pos)
    {
        // this is somewhat an issue since the id is not anymore global like that
        _id = sp._id; // global

        for (auto t : sp._tweens)
        {
            addTween(std::make_shared<Tweenable>(*t));
        }

        if (sp._animation != nullptr)
        {
            setAnimation(std::make_shared<Tweenable>(*(sp._animation)));
        }
        else
        {
            _animation = nullptr;
        }
    }

    Sprite::Sprite(const tmx_spriteobject* so, std::shared_ptr<Imagesheet> is)
    {
        _id = so->id; // global
        _type_name = so->type;
        _pos.x = so->x;
        _pos.y = so->y;
        _size.w = so->width;
        _size.h = so->height;
        _visible = so->visible;
        _img_index = so->tile;
        _imagesheet = is;
        _last_pos = _pos;

        if(so->transform.test(0))
        {
            _transform |= blit::SpriteTransform::HORIZONTAL;
        }
        if(so->transform.test(1))
        {
            _transform |= blit::SpriteTransform::VERTICAL;
        }
    }

    void Sprite::addTween(spTweenable tw)
    {
        tw->init(this);
        _tweens.push_front(tw);
    }

    void Sprite::updateTweens(uint32_t dt)
    {
        auto tw = _tweens.begin();
        while (tw != _tweens.end())
        {
            if (!(*tw))
            {
                // somehow a stale tween
                tw = _tweens.erase(tw);
            }
            else
            {
                (*tw)->update(this, dt);
                if ((*tw)->isFinished())
                {
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
            _animation->update(this, dt);
            if (_animation->isFinished())
            {
                _animation.reset();
            }
        }
    }

    void Sprite::removeTween(const spTweenable& tw, bool suppressCB)
    {
        tw->finish(suppressCB);
        _tweens.remove(tw);
    }

    void Sprite::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            spTweenable& tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool Sprite::tweenActive(const spTweenable &tw)
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


    void Sprite::setAnimation(spTweenable twa)
    {
        _animation = twa;
        _animation->init(this);
    }

    spTweenable Sprite::removeAnimation(bool suppressCB)
    {
        if (_animation)
        {
            _animation->finish(suppressCB);
            _animation->reset();
        }
        return _animation;
    }

    spTweenable Sprite::swapAnimation(spTweenable new_anim, bool suppressCB)
    {
        spTweenable ret = removeAnimation(suppressCB);
        _animation = new_anim;
        return ret;
    }

    blit::Rect Sprite::getBlitRect()
    {
        blit::Rect sr = _imagesheet->getBlitRect(_img_index);
        return sr;
    }

    // Behaviour Tree Stuff
    BTNodeStatus Sprite::BTLoiter(std::shared_ptr<Sprite> s)
    {
//        std::cout << "loitering: " << s->_pos.x << std::endl;
        return BTNodeStatus::SUCCESS;
    }

    BTNodeStatus Sprite::BTIsHeroAround(std::shared_ptr<Sprite> s)
    {
//        std::cout << "is hero around: " << s->_pos.x << std::endl;
        return BTNodeStatus::SUCCESS;
    }

    BTNodeStatus Sprite::BTSleep1Sec(std::shared_ptr<Sprite> s)
    {
//        std::cout << "is sleeping 1 sec: " << s->_pos.x << std::endl;
        return BTNodeStatus::FAILURE;
    }

    void Sprite::setTreeState(std::shared_ptr<TreeState> ts)
    {
        _btTreeState = move(ts);
    }

    std::shared_ptr<TreeState>& Sprite::getTreeState()
    {
        return _btTreeState;
    }
}
