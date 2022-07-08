// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ImgSpr.hpp"
#include "tween/Tweenable.hpp"

#include <32blit.hpp>
#include <src/RectT.hpp>

namespace dang
{
    ImgSpr::ImgSpr() : SpriteObject(), ImageObject()
    {
    }

    ImgSpr::~ImgSpr()
    {
    }

    ImgSpr::ImgSpr(const ImgSpr &s) : SpriteObject(), ImageObject()
    {

    }

    ImgSpr::ImgSpr(const tmx_spriteobject* so, const spImagesheet& is) : SpriteObject(so), ImageObject(so, is)
    {
    }

    void ImgSpr::coreUpdate(uint32_t dt)
    {
    }

    void ImgSpr::render(int32_t vpx, int32_t vpy)
    {
//         && vp.intersects((*it)->getSizeRectG())
        if (isVisible() && isActive() && inZone() /*&& getImagesheet() != nullptr*/)
        {
            Vector2F p = local2Global(getPos());
            blit::Point dp;
            dp.x  = int32_t(std::floor(p.x) - vpx);
            dp.y  = int32_t(std::floor(p.y) - vpy);
            blit::screen.blit(getSurface(), getBlitRect(), dp, getTransform());
        }
    }

}