// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "FullImgSpr.hpp"
#include "tween/Tweenable.hpp"

#include <cassert>

namespace dang
{
    FullImgSpr::FullImgSpr() : ImgSpr(), MotionObject(), TweenObject()
    {

    }

    FullImgSpr::FullImgSpr(const FullImgSpr &s) : ImgSpr(s), MotionObject(s), TweenObject(s)
    {
        // init animation
        if (getAnimation() != nullptr)
        {
            FullImgSpr::visitInit(*getAnimation().get());
        }

        // init tweens
        for (const auto& t : getTweenList())
        {
            FullImgSpr::visitInit(*t.get());
        }

    }

    FullImgSpr::FullImgSpr(const tmx_spriteobject *so, const spImagesheet &is) : ImgSpr(so, is), MotionObject(), TweenObject()
    {

    }

    void FullImgSpr::coreUpdate(uint32_t dt)
    {
        updateTweens(dt);
        SpriteObject::setPos(getPos() + updateMotion(dt));
        updateAnimation(dt);
    }

    void FullImgSpr::visitInit(Tweenable &tw)
    {
        tw.init(*this);
    }

    void FullImgSpr::visitUpdate(Tweenable &tw, uint32_t dt)
    {
        tw.update(*this, dt);
    }

}