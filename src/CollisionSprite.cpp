// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "CollisionSprite.hpp"
#include "CollisionSpriteLayer.hpp"
#include "TmxExtruder.hpp"

namespace dang
{
    CollisionSprite::CollisionSprite() : Sprite(), CollisionObject()
    {

    }

    CollisionSprite::CollisionSprite(const CollisionSprite &cs)
    : Sprite(cs), CollisionObject(cs)
    {
        // clone the BT as well, but with a new state object
        if(cs._nTreeState != nullptr && cs._nTreeState->_tree != nullptr)
        {
            setNTreeState(std::make_shared<dang::NTreeState>(cs._nTreeState->_tree));
        }
    }

    CollisionSprite::~CollisionSprite()
    {
//        std::cout << "Collision sprite destroyed. Type=" << _type_name << std::endl;
#ifdef DANG_DEBUG
        std::cout << "Collision sprite destroyed" << std::endl;
#endif
    }

    CollisionSprite::CollisionSprite(const tmx_spriteobject* so, const spImagesheet& is) : Sprite(so, is)
    {
        _hotrect = {0, 0, float(so->width), float(so->height)};
        _cs_pos = _pos_g;
    }

    /**
     *
     * @return hotrect with global coords of the layer
     */
    RectF CollisionSprite::getHotrectG() const
    {
        return {_cs_pos.x + _hotrect.x, _cs_pos.y + _hotrect.y, _hotrect.w, _hotrect.h};
    }

    /**
     *
     * @return hotrect in local coords of the sprite
     */
    RectF CollisionSprite::getHotrectL() const
    {
        return RectF(_pos.x + _hotrect.x, _pos.y + _hotrect.y, _hotrect.w, _hotrect.h);
    }

    /**
     *
     * @return hotrect relative to (0,0) of the image
     */
    RectF CollisionSprite::getHotrect() const
    {
        return _hotrect;
    }

    void CollisionSprite::collide(const manifold &mf)
    {
        switch (_cr)
        {
            case CR_BOUNCE:
                if (mf.me.get() == this)
                {
                    if (mf.normalMe.x * _vel.x > 0)
                    {
                        _vel.x = -_vel.x;
                    }
                    else if (mf.normalMe.y * _vel.y > 0)
                    {
                        _vel.y = -_vel.y;
                    }

                }
                else
                {
                    if (mf.normalOther.x * _vel.x > 0)
                    {
                        _vel.x = -_vel.x;
                    }
                    else if (mf.normalOther.y * _vel.y > 0)
                    {
                        _vel.y = -_vel.y;
                    }

                }
                break;
            case CR_SLIDE:
                if (mf.normalMe.x != 0)
                {
                    _vel.y = 0;
                }
                else
                {
                    _vel.x = 0;
                }
                break;
            case CR_TOUCH:
                _vel = {0,0};
                break;
        }
    }

/*    void CollisionSprite::slide(const CollisionSpriteLayer::manifold &mf)
    {
        if (_coll_object_type == CollisionSpriteLayer::COT_DYNAMIC)
        {
            if (mf.me.get() == this)
            {
                if (mf.normalMe.x * mf.me->getPosDelta().x > 0)
                {
                    _pos.x = mf.touchMe.x;
                }
                else if (mf.normalMe.y * mf.me->getPosDelta().y > 0)
                {
                    _pos.y = mf.touchMe.y;
                }
            }
            else
            {
                if (mf.normalOther.x * mf.other->getPosDelta().x > 0)
                {
                    _pos.x = mf.touchOther.x;
                }
                else if (mf.normalOther.y * mf.other->getPosDelta().y > 0)
                {
                    _pos.y = mf.touchOther.y;
                }
            }

            spSprite par = _parent.lock();
            if (par != nullptr)
            {
                _pos_g = _pos + par->getPosG();
            }
        }
    }

    void CollisionSprite::touch(const CollisionSpriteLayer::manifold &mf)
    {
        if (_coll_object_type == CollisionSpriteLayer::COT_DYNAMIC)
        {
            if (mf.me.get() == this)
            {
                _pos = mf.touchMe;
            }
            else
            {
                _pos = mf.touchOther;
            }
        }

        spSprite par = _parent.lock();
        if (par != nullptr)
        {
            _pos_g = _pos + par->getPosG();
        }

    }

    void CollisionSprite::bounce(const CollisionSpriteLayer::manifold &mf)
    {
        if (_coll_object_type == CollisionSpriteLayer::COT_DYNAMIC)
        {
            if (mf.me.get() == this)
            {
                if (mf.normalMe.x != 0)
                {
                    float d_bounce = _pos.x - _last_pos.x - mf.deltaMe.x;
                    _pos.x = mf.touchMe.x - d_bounce;
                }
                else
                {
                    float d_bounce = _pos.y - _last_pos.y - mf.deltaMe.y;
                    _pos.y = mf.touchMe.y - d_bounce;
                }
            }
            else    // for the other
            {
                if (mf.normalOther.x != 0)
                {
                    float d_bounce = _pos.x - _last_pos.x - mf.deltaOther.x;
                    _pos.x = mf.touchOther.x - d_bounce;
                }
                else
                {
                    float d_bounce = _pos.y - _last_pos.y - mf.deltaOther.y;
                    _pos.y = mf.touchOther.y - d_bounce;
                }
            }
        }

        spSprite par = _parent.lock();
        if (par != nullptr)
        {
            _pos_g = _pos + par->getPosG();
        }

    }
*/
/*    CollisionSpriteLayer::eCollisionResponse CollisionSprite::getCollisionResponse(const spCollisionSprite& other)
    {
        return _coll_response;
    }
*/
    void CollisionSprite::preSolve()
    {
        _goal = _pos_g;
    }

    void CollisionSprite::postSolve()
    {
        if (spSprite spr = _parent.lock())
        {
            _pos = _goal - spr->getPosG();
        }
        else
        {
            _pos = _goal;
        }
    }

    void CollisionSprite::update(uint32_t dt)
    {

    }

    void CollisionSprite::setNTreeState(std::shared_ptr<NTreeState> ts)
    {
        _nTreeState = move(ts);
    }

    std::shared_ptr<NTreeState>& CollisionSprite::getNTreeState()
    {
        return _nTreeState;
    }

    void CollisionSprite::markRemove()
    {
        _remove_from_layer = true;
        _remove_from_cs = true;
    }

    void CollisionSprite::addSprite(spSprite s)
    {
        Sprite::addSprite(s);
        spCollisionSprite cs = std::static_pointer_cast<CollisionSprite>(s);
        cs->_cs_pos = _pos_g;
    }

}