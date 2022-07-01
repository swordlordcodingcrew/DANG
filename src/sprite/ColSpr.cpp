// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ColSpr.hpp"
#include "TmxExtruder.hpp"
#include "dang.hpp"

#ifdef DANG_DEBUG_DRAW
#include <engine/engine.hpp>
#endif

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

    void ColSpr::render(int32_t vpx, int32_t vpy)
    {
#ifdef DANG_DEBUG_DRAW
        // show hotrects
        if (isActive() && inZone())
        {
            blit::screen.pen = blit::Pen(0, 0, 255, 255);
            RectF hr;
            hr.x = _co_pos.x + _hotrect.x - vpx;
            hr.y = _co_pos.y + _hotrect.y - vpy;
            hr.w = _hotrect.w;
            hr.h = _hotrect.h;

            blit::Point tl(int32_t(hr.tl().x), int32_t(hr.tl().y));
            blit::Point bl(int32_t(hr.bl().x), int32_t(hr.bl().y));
            blit::Point br(int32_t(hr.br().x), int32_t(hr.br().y));
            blit::Point tr(int32_t(hr.tr().x), int32_t(hr.tr().y));

            blit::screen.line(tl, bl); // left -> bottom
            blit::screen.line(bl, br); // bottom -> right
            blit::screen.line(br, tr); // right -> top
            blit::screen.line(tr, tl); // top -> left

        }
#endif

    }

}