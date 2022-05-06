// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "CollisionSprite.hpp"
#include "CollisionSpriteLayer.hpp"
#include "TmxExtruder.hpp"
#include "bt/NTree.h"

#include <iostream>
#include <cassert>

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
        _co_pos = local2Global(_pos);
//        _cs_pos = _pos_g;
    }

    /**
     *
     * @return hotrect with global coords of the layer
     */
    RectF CollisionSprite::getHotrectG() const
    {
        return {_co_pos.x + _hotrect.x, _co_pos.y + _hotrect.y, _hotrect.w, _hotrect.h};
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

    void CollisionSprite::preSolve()
    {
        _goal = local2Global(_pos);
    }

    void CollisionSprite::postSolve()
    {
        _pos = global2Local(_goal);
    }

    void CollisionSprite::update(uint32_t dt)
    {
        if (_nTreeState != nullptr)
        {
            assert(_nTreeState->_tree != nullptr);
            _nTreeState->_tree->process(*this, _nTreeState, dt);

#ifdef DANG_DEBUG_PRINT
            std::cout << "tree processed with status: " << +static_cast<std::underlying_type_t<dang::NTreeState::internal_state>>(_nTreeState->_internal_state) << " and node position: " << _nTreeState->_node << std::endl;
#endif
        }
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
//        cs->_cs_pos = _pos_g;
        cs->_co_pos = local2Global(_pos);
    }

    void CollisionSprite::setPos(const Vector2F &pos)
    {
        Sprite::setPos(pos);
        _co_pos = local2Global(pos);
    }

    void CollisionSprite::setPosX(float x)
    {
        Sprite::setPosX(x);
//        _cs_pos.x = x + getParentPos().x;
        _co_pos.x = local2Global({x, 0}).x;
    }

    void CollisionSprite::setPosY(float y)
    {
        Sprite::setPosY(y);
//        _cs_pos.y = y + getParentPos().y;
        _co_pos.y = local2Global({0, y}).y;
    }

    spCollisionSprite CollisionSprite::getOther(const manifold& mf, const CollisionSprite* me)
    {
        const CollisionSprite* mf_other = dynamic_cast<CollisionSprite*>(mf.other.get());
        const CollisionSprite* mf_me = dynamic_cast<CollisionSprite*>(mf.me.get());

        if (me == mf_me)
        {
            return std::dynamic_pointer_cast<CollisionSprite>(mf.other);
        }
        else if (me == mf_other)
        {
            return std::dynamic_pointer_cast<CollisionSprite>(mf.me);
        }

        std::cout << "error in manifold me/other" << std::endl;
        return nullptr;

    }

}