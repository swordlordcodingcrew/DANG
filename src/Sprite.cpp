// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#ifdef DANG_SPR

#include "Sprite.hpp"
#include "TmxExtruder.hpp"
#include "tween/Tweenable.hpp"
#include "Imagesheet.hpp"

#include <32blit.hpp>

#include <algorithm>

#include <iostream>
#include <cassert>

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
      _acc(sp._acc), _gravity(sp._gravity)
    {
        // this is somewhat an issue since the id is not anymore global like that
        _id = sp._id; // global

        for (const auto& t : sp._tweens)
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

    RectF Sprite::getSizeRectG()
    {
        Vector2F p = local2Global(_pos);
        return RectF(p.x, p.y, _size.x, _size.y);
    }

    void Sprite::render(int32_t vpx, int32_t vpy)
    {
        Vector2F p = local2Global(_pos);
        blit::Point dp;
        dp.x  = int32_t(std::floor(p.x) - vpx);
        dp.y  = int32_t(std::floor(p.y) - vpy);
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
        s->_parent = this;//shared_from_this();

        if (_child == nullptr)  // first child
        {
            D_DEBUG_PRINT("add sprite as first child\n");
            _child = s;
            _child->_next_sibling = nullptr;
            _child->_prev_sibling = nullptr;
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
                _child->_prev_sibling = nullptr;

                sp->_prev_sibling = _child.get();
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
                    s->_prev_sibling = sp.get();
                    s->_next_sibling = nullptr;
                }
                else
                {
                    // somewhere in between
                    D_DEBUG_PRINT("add sprite in between\n");
                    assert(sp->_prev_sibling != nullptr);
                    sp->_prev_sibling->_next_sibling = s;

                    s->_next_sibling = sp;
                    s->_prev_sibling = sp->_prev_sibling;

                    sp->_prev_sibling = s.get();
                }
            }
        }
    }

    /**
     * removes this sprite from the tree (and its children)
     */
    void Sprite::removeMeFromTree()
    {
        D_DEBUG_PRINT("removeMeFromTree\n");
        if (_parent != nullptr)
        {
            D_DEBUG_PRINT("removeMeFromTree: spr locked\n");
            if (_prev_sibling == nullptr)
            {
                // first child
                if (_next_sibling == nullptr)
                {
                    // only child
                    _parent->_child = nullptr;
                    D_DEBUG_PRINT("removeMeFromTree: remove only child\n");
                }
                else
                {
                    _parent->_child = _next_sibling;
                    _parent->_child->_prev_sibling = nullptr;
                    D_DEBUG_PRINT("removeMeFromTree: remove first child\n");
                }
            }
            else
            {
                if (_next_sibling != nullptr)
                {
                    _prev_sibling->_next_sibling = _next_sibling;
                    _next_sibling->_prev_sibling = _prev_sibling;
                    D_DEBUG_PRINT("removeMeFromTree: remove a middle sibling\n");
                }
                else
                {
                    _prev_sibling->_next_sibling.reset();
                    D_DEBUG_PRINT("removeMeFromTree: remove last sibling\n");
                }
            }

            _parent = nullptr;
            _next_sibling.reset();
            _prev_sibling = nullptr;
        }
        else
        {
            // sprite is not in a tree;
            // (and the root sprite cannot be removed)
        }
    }

    Vector2F Sprite::local2Global(const Vector2F &in)
    {
        Vector2F ret = in;
        Sprite* par = _parent;
        while (par != nullptr)
        {
            ret += par->_pos;
            par = par->_parent;
        }
        return ret;
    }

    Vector2F Sprite::global2Local(const Vector2F &in)
    {
        Vector2F ret = in;
        Sprite* par = _parent;
        while (par != nullptr)
        {
            ret -= par->_pos;
            par = par->_parent;
        }
        return ret;
    }
}
#endif