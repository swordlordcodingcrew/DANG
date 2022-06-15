// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ColSpr.hpp"
#include "TmxExtruder.hpp"
#include "dang.hpp"

namespace dang
{

    ColSpr::ColSpr() : SpriteObject(), CollisionObject()
    {
    }

    ColSpr::ColSpr(const ColSpr &cs) : SpriteObject(cs), CollisionObject(cs)
    {

    }

    ColSpr::ColSpr(const tmx_spriteobject *so) : SpriteObject(so)
    {
        _hotrect = {0, 0, float(so->width), float(so->height)};
        _co_pos = local2Global(getPos());

    }

    ColSpr::~ColSpr()
    {
        D_DEBUG_PRINT("Collision sprite destroyed\n");
    }

    /**
 *
 * @return hotrect with global coords of the layer
 */
    RectF ColSpr::getHotrectG() const
    {
        return {_co_pos.x + _hotrect.x, _co_pos.y + _hotrect.y, _hotrect.w, _hotrect.h};
    }

    /**
     *
     * @return hotrect in local coords of the sprite
     */
    RectF ColSpr::getHotrectL() const
    {
        return RectF(getPos().x + _hotrect.x, getPos().y + _hotrect.y, _hotrect.w, _hotrect.h);
    }

    /**
     *
     * @return hotrect relative to (0,0) of the image
     */
    RectF ColSpr::getHotrect() const
    {
        return _hotrect;
    }

    void ColSpr::collide(const manifold &mf)
    {
/*        switch (_cr)
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
*/    }

    void ColSpr::preSolve()
    {
        _goal = local2Global(getPos());
    }

    void ColSpr::postSolve()
    {
        setPos(global2Local(_goal));
    }

    void ColSpr::markRemove()
    {
        SpriteObject::markRemove();
        _remove_from_cs = true;
    }

    void ColSpr::addSpriteObject(spSpriteObject s)
    {
        SpriteObject::addSpriteObject(s);
        spColSpr cs = std::static_pointer_cast<ColSpr>(s);
        cs->_co_pos = local2Global(getPos());
    }

    void ColSpr::setPos(const Vector2F &pos)
    {
        SpriteObject::setPos(pos);
        _co_pos = local2Global(pos);
    }

    void ColSpr::setPosX(float x)
    {
        SpriteObject::setPosX(x);
        _co_pos.x = local2Global({x, 0}).x;
    }

    void ColSpr::setPosY(float y)
    {
        SpriteObject::setPosY(y);
        _co_pos.y = local2Global({0, y}).y;
    }

    spColSpr ColSpr::getOther(const manifold& mf, const ColSpr* me)
    {
        const ColSpr* mf_other = dynamic_cast<ColSpr*>(mf.other.get());
        const ColSpr* mf_me = dynamic_cast<ColSpr*>(mf.me.get());

        if (me == mf_me)
        {
            return std::dynamic_pointer_cast<ColSpr>(mf.other);
        }
        else if (me == mf_other)
        {
            return std::dynamic_pointer_cast<ColSpr>(mf.me);
        }

        D_DEBUG_PRINT("error in manifold me/other");
        return nullptr;

    }

}