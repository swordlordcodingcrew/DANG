// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Sprite.hpp"
#include "TmxExtruder.hpp"
#include "tween/Tweenable.hpp"
#include "Imagesheet.hpp"

#include <32blit.hpp>

#include <algorithm>

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

    Sprite::Sprite(const tmx_spriteobject* so, const spImagesheet& is)
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
        _z_order = so->z_order;
        _last_pos = _pos;
        _pos_g = _pos;
        _last_pos_g = _pos;

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
        if (tw != nullptr)
        {
            tw->finish(suppressCB);
            _tweens.remove(tw);
        }
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
        _last_pos_g = _pos_g;
        updateTweens(dt);

        // dt in 10 ms
        float dt10ms = dt / 100.0f;
        _vel += (_gravity + _acc) * dt10ms;
        _pos += _vel * dt10ms;
        spSprite par = _parent.lock();
        if (par != nullptr)
        {
            _pos_g = _pos + par->_pos_g;
        }
    }

    void Sprite::update(uint32_t dt)
    {
    }

    RectF Sprite::getSizeRect()
    {
        return RectF(_pos.x, _pos.y, _size.x, _size.y);
    }

    RectF Sprite::getSizeRectG()
    {
        return RectF(_pos_g.x, _pos_g.y, _size.x, _size.y);
    }

    void Sprite::render(int32_t vpx, int32_t vpy)
    {
        blit::Point dp;
        dp.x  = int32_t(std::floor(_pos_g.x) - vpx);
        dp.y  = int32_t(std::floor(_pos_g.y) - vpy);
        blit::screen.blit(_imagesheet->getSurface(), getBlitRect(), dp, _transform);
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
        return std::move(_animation);
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

    blit::Rect Sprite::getBlitRect(const uint16_t img_index)
    {
        blit::Rect sr = _imagesheet->getBlitRect(img_index);
        return sr;
    }

    // configures speed so that the sprite targets "target" (aka move directly to)
    void Sprite::setVelTowardsPoint(const Vector2F& target)
    {
        setVelTowardsPoint(target, _vel.x, _vel.y);
    }

    void Sprite::setVelTowardsPoint(const Vector2F& target, const float speedx, const float speedy)
    {
        auto sourcex = _pos.x;
        auto sourcey = _pos.y;

        auto targetx = target.x;
        auto targety = target.y;

        // auto aim hero
        auto angle_rad = atan2(targetx - sourcex, targety - sourcey);

        setVelRelToRadAngle(angle_rad, speedx, speedy);
    }

    void Sprite::setVelRelToRadAngle(const double angle)
    {
        setVelRelToRadAngle(angle, _vel.x, _vel.y);
    }

    void Sprite::setVelRelToRadAngle(const double angle, const float speedx, const float speedy)
    {
        setVelX(speedx * (float)sin(angle) * 1);
        setVelY(speedy * (float)cos(angle) * 1);
    }

    /** adds a sprite as child to this sprite
     *
     * @param s sprite to be added as child
     */
    void Sprite::addSprite(spSprite s)
    {
        s->_parent = shared_from_this();

        if (_child == nullptr)  // first child
        {
            D_DEBUG_PRINT("add sprite as first child\n");
            _child = s;
            _child->_next_sibling = nullptr;
            _child->_prev_sibling.reset();
        }
        else
        {
            // go through siblings until z_order is correct
            spSprite sp = _child;
            if (sp->_z_order >= s->_z_order)
            {
                // first position
                D_DEBUG_PRINT("add sprite at first position\n");
                _child = s;
                _child->_next_sibling = sp;
                _child->_prev_sibling.reset();

                sp->_prev_sibling = _child;
            }
            else
            {
                while (sp->_next_sibling != nullptr)
                {
                    if (sp->_z_order >= s->_z_order)
                    {
                        break;
                    }
                    sp = sp->_next_sibling;
                }

                if (sp->_next_sibling == nullptr)
                {
                    // last sibling
                    D_DEBUG_PRINT("add sprite at last position\n");
                    sp->_next_sibling = s;
                    s->_prev_sibling = sp;
                    s->_next_sibling = nullptr;
                }
                else
                {
                    // somewhere in between
                    D_DEBUG_PRINT("add sprite in between\n");
                    spSprite prev = sp->_prev_sibling.lock();
                    if (prev != nullptr)
                    {
                        prev->_next_sibling = s;

                        s->_next_sibling = sp;
                        s->_prev_sibling = prev;

                        sp->_prev_sibling = s;
                    }
                    else
                    {
                        D_DEBUG_PRINT("could not add sprite error\n");
                    }
                }
            }
        }
        s->_pos_g = s->_pos + _pos;
        s->_last_pos = s->_pos;
        s->_last_pos_g = s->_pos_g;
    }

    /**
     * removes this sprite from the tree (and its children)
     */
    void Sprite::removeMeFromTree()
    {
        D_DEBUG_PRINT("removeMeFromTree\n");
        spSprite par = _parent.lock();
        if (par != nullptr)
        {
            D_DEBUG_PRINT("removeMeFromTree: spr locked\n");
            spSprite prev = _prev_sibling.lock();
            if (prev == nullptr)
            {
                // first child
                if (_next_sibling == nullptr)
                {
                    // only child
                    par->_child = nullptr;
                    D_DEBUG_PRINT("removeMeFromTree: remove only child\n");
                }
                else
                {
                    par->_child = _next_sibling;
                    par->_child->_prev_sibling.reset();
                    D_DEBUG_PRINT("removeMeFromTree: remove first child\n");
                }
            }
            else
            {
                if (_next_sibling != nullptr)
                {
                    prev->_next_sibling = _next_sibling;
                    _next_sibling->_prev_sibling = prev;
                    D_DEBUG_PRINT("removeMeFromTree: remove a middle sibling\n");
                }
                else
                {
                    prev->_next_sibling.reset();
                    D_DEBUG_PRINT("removeMeFromTree: remove last sibling\n");
                }
            }

            _parent.reset();
            _next_sibling.reset();
            _prev_sibling.reset();
        }
        else
        {
            // sprite is not in a tree;
            // (and the root sprite cannot be removed)
        }
    }

    Vector2F Sprite::getPosG()
    {
        spSprite s = _parent.lock();
        _pos_g = _pos;
        if (s != nullptr)
        {
            _pos_g += s->getPosG();
        }
        return _pos_g;
    }

    Vector2F Sprite::getLastPosG()
    {
        spSprite s = _parent.lock();
        _last_pos_g = _last_pos;
        if (s != nullptr)
        {
            _last_pos_g += s->getLastPosG();
        }
        return _last_pos_g;
    }


}
