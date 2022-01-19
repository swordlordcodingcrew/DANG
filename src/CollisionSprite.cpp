// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "CollisionSprite.hpp"
#include "CollisionSpriteLayer.hpp"
#include "TmxExtruder.hpp"

namespace dang
{
    CollisionSprite::CollisionSprite() : Sprite()
    {

    }

    CollisionSprite::CollisionSprite(const CollisionSprite &cs)
    : Sprite(cs), _coll_object_type(cs._coll_object_type), _hotrect(cs._hotrect), _coll_response(cs._coll_response)
    {

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
    }

    /**
     *
     * @return hotrect relative to (0,0) of the image
     */
    RectF CollisionSprite::getHotrect() const
    {
        return _hotrect;
    }

    /**
     *
     * @return hotrect in local coords of the sprite
     */
    RectF CollisionSprite::getHotrectL() const
    {
        return RectF(_hotrect.x + _pos.x, _hotrect.y + _pos.y, _hotrect.w, _hotrect.h);
    }

    /**
     *
     * @return hotrect with global coords of the layer
     */
    RectF CollisionSprite::getHotrectG() const
    {
        return RectF(_hotrect.x + _pos_g.x, _hotrect.y + _pos_g.y, _hotrect.w, _hotrect.h);
    }

    void CollisionSprite::collide(const CollisionSpriteLayer::manifold &mf)
    {
        switch (_coll_response)
        {
            case CollisionSpriteLayer::CR_BOUNCE:
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
            case CollisionSpriteLayer::CR_SLIDE:
                if (mf.normalMe.x != 0)
                {
                    _vel.y = 0;
                }
                else
                {
                    _vel.x = 0;
                }
                break;
            case CollisionSpriteLayer::CR_TOUCH:
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
        }
    }

    void touch(const CollisionSpriteLayer::manifold &mf)
    {

    }

    void bounce(const CollisionSpriteLayer::manifold &mf)
    {

    }

*/

    CollisionSpriteLayer::eCollisionResponse CollisionSprite::getCollisionResponse(const spCollisionSprite& other)
    {
        return _coll_response;
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

}